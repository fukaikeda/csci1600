#include "fsm.h"

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize
  
  // initialize each component
  initButtons();

  // savedClock = millis();
}

void loop() {
  static state CURRENT_STATE = sDisplayRealTime;

  if (triggeredUserButton != -1) {
    Serial.print("User button triggered ISR: ");
    Serial.println(triggeredUserButton);
    // triggeredUserButton = -1; // Reset after processing
  }

  updateActionButtonInputs(); // polling button inputs
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  delay(10);
}

state updateFSM(state curState, long mils) {
  static bool pwm_finished_fsm = false;
  static bool message_finished_fsm = false;
  state nextState;
  switch(curState) {
  case sDisplayRealTime: // state 0
    if (selectedUser != user.none) {
      turnOnLED(userLEDs[selectedUser]);
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // state 1
    if (selectedAction != action.noAction && mils - savedClock > 10000) {
      // go back to state 0
      nextState = sDisplayRealTime;
      turnOffLED(userLEDs[selectedUser]);
    } else if (selectedAction == action.returnTime) {
      //displayReturnTime();            
      turnOnLED(actionLEDs[selectedAction]);
      ////////// some way to ignore additional user/message button inputs
      nextState = sWaitAfterTimeBut;
      savedClock = mils;
    } else if (selectedAction == action.message) {
      //sendMessages(); 
      turnOnLED(actionLEDs[selectedAction]);
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
      turnOffLED(userLEDs[selectedUser]);
      turnOffLED(actionLEDs[selectedUser]);
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
      turnOffLED(userLEDs[selectedUser]);
      turnOffLED(actionLEDs[selectedUser]);
      resetSelection();
      message_finished = false;
    }
    break;
  }
  return nextState;  
}
