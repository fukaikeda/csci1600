#include "fsm.h"
#include "notifications.h"
#include "gcal.h"
#include "ClockController.h"

Notifications notifManager;
GCal gcalManager;
ClockController clockController;

static int savedClock = 0;
static bool message_finished = false;
static bool clock_finished = false;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize
  
  // initialize each component
  initButtons();
  notifManager.initNotifications();
  notifManager.setupWiFi();
  gcalManager.initGCal();
  gcalManager.fetchData();
  clockController.initClock();
  // gcalManager.connectWiFi()
 }

void loop() {
  static State CURRENT_STATE = sDisplayRealTime;
  // updateActionButtonInputs(); // polling button inputs
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  // Serial.println(CURRENT_STATE);
  delay(10);
}

State updateFSM(State curState, long mils) {
  // Serial.print("User button selected: ");
  // Serial.println(triggeredUserButton);

  State nextState;
  switch(curState) {
  case sDisplayRealTime: // state 0
    Serial.println("State 0");
    if (triggeredUserButton != User::None) { // Transition 0-1
      turnOnLED(userLED);
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // state 1
    Serial.println("State 1");
    if (mils - savedClock < 10000) {
      if (triggeredActionButton == Action::ReturnTime) { // Transition 1-2
        noInterrupts();  // disable additional button inputs
        // fetch the return time
        gcalManager.fetchData();
        String time = gcalManager.getHomeTime(names[triggeredUserButton]);

        Serial.print("Return time for ");
        Serial.println(names[triggeredUserButton]);
        Serial.println(time);

        clock_finished = clockController.handleInputMode(time);   
        while(!clock_finished); /// not sure if we need this        
        turnOnLED(actionLED);
        savedClock = mils;
        nextState = sWaitAfterTimeBut;
      } else if (triggeredActionButton == Action::Message) { // Transition 1-3
        noInterrupts(); // disable additional button inputs
        message_finished = notifManager.sendEncouragingMessage(names[triggeredUserButton]);
        turnOnLED(actionLED);
        savedClock = mils;
        nextState = sWaitAfterMessage; 
      }
    } else {   // Transition 1-0
      resetSelection();
      savedClock = mils;
      nextState = sDisplayRealTime;
    }
    break;
  case sWaitAfterTimeBut: // state 2
    Serial.println("State 2");
    if (clock_finished) {
      if (mils - savedClock > 10000) {  // Transition 2-0
        clockController.handleRealTimeMode();     
        resetSelection();
        clock_finished = false;
        interrupts();
        nextState = sDisplayRealTime;
      }
    }
    break;
  case sWaitAfterMessage: // state 3 
    Serial.println("State 3");
    if (mils - savedClock > 5000) {
      if (!message_finished) {  // Transition 3-0(b)
        indicateError();
        Serial.println("error occured while sending a message");
      }
      clockController.handleRealTimeMode();  
      resetSelection();
      message_finished = false;
      interrupts();
      nextState = sDisplayRealTime;
    }
    break;
  }
  return nextState;  
}
