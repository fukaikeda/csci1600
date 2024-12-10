#include "fsm.h"
// #include "notifications/notifications.h"
#include "notifications.h"
// #include "gcal/gcal.h"
#include "gcal.h"

Notifications notifManager;
GCal gcalManager;

static int savedClock;
static bool message_finished = false;

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
  gcalManager.fetchData();
  CURRENT_STATE = updateFSM(CURRENT_STATE, millis());
  delay(10);
}

State updateFSM(State curState, long mils) {
  Serial.print("User button selected: ");
  Serial.println(triggeredUserButton);

  State nextState;
  switch(curState) {
  case sDisplayRealTime: // state 0
    resetSelection();
    if (triggeredUserButton != User::None) {
      turnOnLED(userLED);
      nextState = sWaitAfterUserBut;
      savedClock = mils;
    }
    break;
  case sWaitAfterUserBut: // state 1
    if (triggeredActionButton != Action::NoAction && mils - savedClock > 10000) {
      // go back to state 0
      nextState = sDisplayRealTime;
      turnOffLED(userLED);
    } else if (triggeredActionButton == Action::ReturnTime) {
      String time = gcalManager.getHomeTime(names[triggeredUserButton]);
      //displayReturnTime();            
      turnOnLED(actionLED);
      ////////// some way to ignore additional user/message button inputs
      nextState = sWaitAfterTimeBut;
      savedClock = mils;
    } else if (triggeredActionButton == Action::Message) {
      notifManager.sendEncouragingMessage(names[triggeredUserButton]);
      turnOnLED(actionLED);
      ////////// some way to ignore additional user/returnTime button inputs
      nextState = sWaitAfterMessage; 
      savedClock = mils;
    }
    break;
  case sWaitAfterTimeBut: // state 2
    if (mils - savedClock > 10000) {
      resetSelection();
    }
    break;
  case sWaitAfterMessage: // state 3 
    if(mils - savedClock > 3000) {
      resetSelection();
      message_finished = false;
    }
    break;
  }
  return nextState;  
}
