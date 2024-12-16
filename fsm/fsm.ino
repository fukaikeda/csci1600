// #define MOCK
// #define RUN_TESTS

#include "fsm.h"
#include "MyWDT.h"

volatile bool userLEDState = LOW;
volatile bool actionLEDState = LOW;

#ifdef MOCK
#include "mock.h"

mockNotifications notifManager;
mockCalendar gcalManager;
mockClock clockController;

#else
#include "notifications.h"
#include "gcal.h"
#include "ClockController.h"

Notifications notifManager;
GCal gcalManager;
ClockController clockController;

#endif

#ifndef RUN_TESTS
void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize
  Serial.println("initalized");
  
  // initialize each component
  
  initButtons();
  notifManager.initNotifications();
  notifManager.setupWiFi();
  gcalManager.initGCal();
  gcalManager.fetchData();
  clockController.initClock(); 
  setupWDT(); 
  petWatchdog(); 
  // Serial.println("set up ended");

}

void loop() {
  petWatchdog(); 
  static State CURRENT_STATE = sDisplayRealTime;
  // Serial.println("loop beginning");
  if (clockController.displayRealTime){
    petWatchdog(); 
    clockController.handleRealTimeMode();  
    petWatchdog(); 
  }  
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  petWatchdog(); 
  delay(10); // Delay is added to avoid redundant updates
}

#endif

State updateFSM(State curState, long mils) {
  State nextState;
  switch(curState) {
  case sDisplayRealTime: // State 0
    Serial.println("State 0");
    if (triggeredUserButton != User::None) { // Transition 0-1
      turnOnLED(userLED);
      petWatchdog(); 
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // state 1
    // Serial.println("State 1");
    if (mils - savedClock < 10000) {
      if (triggeredActionButton == Action::ReturnTime) { // Transition 1-2
        // fetch the return time
        petWatchdog();
        gcalManager.fetchData();
        petWatchdog(); // Reset WDT after successful fetch

        String time = gcalManager.getHomeTime(names[triggeredUserButton]);
        Serial.print("Return time for ");
        Serial.println(names[triggeredUserButton]);
        Serial.println(time);

        clockController.displayRealTime = false; 
        clockController.handleInputMode(time);           
        turnOnLED(actionLED);
        savedClock = mils;
        nextState = sWaitAfterTimeBut;
        petWatchdog();
      } else if (triggeredActionButton == Action::Message) { // Transition 1-3
        petWatchdog(); // Reset WDT after successful action
        notifManager.sendEncouragingMessage(names[triggeredUserButton]);
        turnOnLED(actionLED);
        savedClock = mils;
        nextState = sWaitAfterMessage; 
      }
    } else {   // Transition 1-0
      resetSelection();
      petWatchdog(); // Reset WDT after successful action
      savedClock = mils;
      nextState = sDisplayRealTime;
    }
    break;
  case sWaitAfterTimeBut: // state 2
    // Serial.println("State 2");
    if (mils - savedClock > 20000) {  // Transition 2-0
      clockController.displayRealTime = true;     
      Serial.println("Going back to real time FSM");
      resetSelection();
      petWatchdog(); // Reset WDT after successful action
      savedClock = mils;
      nextState = sDisplayRealTime;
    }
    break;
  case sWaitAfterMessage: // state 3 
    // Serial.println("State 3");
    if (mils - savedClock > 5000) {
      if (!notifManager.message_finished) {  // Transition 3-0(b)
        indicateError();
        Serial.println("error occured while sending a message");
      }

      petWatchdog(); // Reset WDT after successful action
      clockController.displayRealTime = true; 
      resetSelection();
      notifManager.message_finished = false;
      savedClock = mils;
      nextState = sDisplayRealTime;
    }
    break;
  }
  return nextState;  
}
