#include "fsm.h"
#include "notifications/notifications.h"
#include "gcal/gcal.h"

Notifications notifManager;
GCal gcalManager;

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize
  
  // initialize each component
  initButtons();
  notifManager.initNotifications();
  notifManager.setupWiFi();
  gcalManager.initGCal();
  // gcalManager.connectWiFi()
 }

void loop() {
  static State CURRENT_STATE = sDisplayRealTime;
  updateActionButtonInputs(); // polling button inputs
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  delay(10);
}

State updateFSM(State curState, long mils) {
  Serial.print("User button selected: ");
  Serial.println(triggeredUserButton);

  static bool pwm_finished_fsm = false;
  static bool message_finished_fsm = false;
  State nextState;
  switch(curState) {
  case sDisplayRealTime: // state 0
    resetSelection();
    if (triggeredUserButton != User::None) {
      turnOnLED(userLEDs[triggeredUserButton]);
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // state 1
    if (triggeredActionButton != Action::NoAction && mils - savedClock > 10000) {
      // go back to state 0
      nextState = sDisplayRealTime;
      turnOffLED(userLEDs[triggeredUserButtonU]);
    } else if (triggeredActionButton == Action::ReturnTime) {
      //displayReturnTime();            
      turnOnLED(actionLEDs[triggeredActionButton]);
      ////////// some way to ignore additional user/message button inputs
      nextState = sWaitAfterTimeBut;
      savedClock = mils;
    } else if (triggeredActionButton == Action::Message) {
      notifManager.sendEncouragingMessage(names[triggeredUserButton]);
      turnOnLED(actionLEDs[triggeredActionButton]);
      ////////// some way to ignore additional user/returnTime button inputs
      nextState = sWaitAfterMessage; 
      savedClock = mils;
    }
    break;
  case sWaitAfterTimeBut: // state 2
    if (pwm_finished) {
      pwm_finished_fsm = true;
      savedClock = mils;
      pwm_finished = false;
    }
    
    if (pwm_finished_fsm && mils - savedClock > 10000) {
      turnOffLED(userLEDs[triggeredUserButton]);
      turnOffLED(actionLEDs[triggeredActionButton]);
      resetSelection();
      pwm_finished_fsm = false;
    }
    break;
  case sWaitAfterMessage: // state 3
    if (message_finished) {
      message_finished_fsm = true;
      savedClock = mils;
      message_finished = false;
    }    
    if(message_finished_fsm && mils - savedClock > 3000) {
      turnOffLED(userLEDs[triggeredUserButton]);
      turnOffLED(actionLEDs[triggeredActionButton]);
      resetSelection();
      message_finished = false;
    }
    break;
  }
  return nextState;  
}
