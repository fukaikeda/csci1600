#include "fsm.h"
#include "notifications.h"
#include "gcal.h"
#include "ClockController.h"

Notifications notifManager;
GCal gcalManager;
ClockController clockController;

static int savedClock = 0;
static bool message_finished = false;

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
  
  // clockController.handleInputMode("1000");
  // while(true){
    
  // }
  // gcalManager.connectWiFi()
 
 }

void loop() {
  static State CURRENT_STATE = sDisplayRealTime;

  if (displayRealTime){
    clockController.handleRealTimeMode();  
  }
  // updateActionButtonInputs(); // polling button inputs
  
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());

  delay(10); // Delay is added to avoid redundant updates
}

State updateFSM(State curState, long mils) {
  // Serial.print("User button selected: ");
  // Serial.println(triggeredUserButton);

  State nextState;
  switch(curState) {
  case sDisplayRealTime: // State 0
    Serial.println("State 0");
    if (triggeredUserButton != User::None) { // Transition 0-1
      turnOnLED(userLED);
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // State 1
    Serial.println("State 1");
    if (mils - savedClock < 10000) {
      if (triggeredActionButton == Action::ReturnTime) { // Transition 1-2
        // fetch the return time
        gcalManager.fetchData();
        String time = gcalManager.getHomeTime(names[triggeredUserButton]);
        Serial.print("Return time for ");
        Serial.println(names[triggeredUserButton]);
        Serial.println(time);

        displayRealTime = false; 
        clockController.handleInputMode(time);           
        turnOnLED(actionLED);
        ////////// some way to ignore additional user/message button inputs
        savedClock = mils;
        nextState = sWaitAfterTimeBut;
      } else if (triggeredActionButton == Action::Message) { // Transition 1-3
        notifManager.sendEncouragingMessage(names[triggeredUserButton]);
        turnOnLED(actionLED);
        ////////// some way to ignore additional user/returnTime button inputs
        savedClock = mils;
        nextState = sWaitAfterMessage; 
      }
    } else {   // Transition 1-0
      resetSelection();
      savedClock = mils;
      nextState = sDisplayRealTime;
    }
    break;
  case sWaitAfterTimeBut: // State 2
    Serial.println("State 2");
    if (mils - savedClock > 20000) {  // Transition 2-0
      displayRealTime = true; 
      clockController.handleRealTimeMode();     
      Serial.println("Going back to real time FSM");
      resetSelection();
      nextState = sDisplayRealTime;
    }
    break;
  case sWaitAfterMessage: // state 3 
    Serial.println("State 3");
    if (mils - savedClock > 5000) {
      if (!message_finished) {  // Transition 3-0(b)
        indicateError();
        Serial.println("error occured");
      }
      displayRealTime = true; 
      clockController.handleRealTimeMode();  
      resetSelection();
      message_finished = false;
      nextState = sDisplayRealTime;
    }
    break;
  }
  return nextState;  
}
