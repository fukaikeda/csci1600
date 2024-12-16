#ifndef FSM_H
#define FSM_H

extern volatile bool userLEDState;
extern volatile bool actionLEDState;

#ifdef MOCK
#include "mock.h"

extern mockNotifications notifManager;
extern mockCalendar gcalManager;
extern mockClock clockController;

#else
#include "notifications.h"
#include "gcal.h"
#include "ClockController.h"

extern Notifications notifManager;
extern GCal gcalManager;
extern ClockController clockController;
#endif

typedef enum {
  User1 = 0,
  User2 = 1,
  User3 = 2,
  User4 = 3,
  None = -1
} User;

typedef enum {
  ReturnTime = 0,
  Message = 1,
  NoAction = -1
} Action;

typedef enum {
  sDisplayRealTime = 0,
  sWaitAfterUserBut = 1,
  sWaitAfterTimeBut = 2,
  sWaitAfterMessage = 3
} State;

const char* names[4] = {"Fuka", "Mia", "Kana", "Jason"};

const int userButtons[4] = {A1, A2, A3, A5}; // User buttons Do not change!!
const int actionButtons[2] = {2, 3};         // Action buttons
const int userLED = 12;                      // LED for user buttons
const int actionLED = 13;                    // LED for action buttons

// Include additional variables to store the triggered button
volatile User triggeredUserButton = User::None; // Variable to track the user button that triggered ISR
volatile Action triggeredActionButton = Action::NoAction; // Variable to track the action button that triggered ISR

int savedClock = 0;
bool message_finished = true;
bool clock_finished = true;

void turnOnLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
}

void turnOffLED(int ledPin) {
  digitalWrite(ledPin, LOW);
}

/*
 * Read in button inputs and light on corresponding LEDs
 */
void updateActionButtonInputs() {
  actionLEDState = LOW; // Reset the action LED state
  for (int i = 0; i < 2; i++) {
    if (digitalRead(actionButtons[i]) == HIGH) { // Button is pressed (active low)
      actionLEDState = HIGH; // Turn on the action LED
      turnOnLED(actionLED);
      triggeredActionButton = (Action) i;
      break; // Exit loop once any button is pressed
    }
  }

  if (actionLEDState == HIGH && triggeredActionButton != Action::NoAction) {
    Serial.print("Action LED triggered by button: ");
    Serial.println(triggeredActionButton);

    // Perform specific actions based on the button
    if (triggeredActionButton == 3) {
      Serial.println("Action LED caused by button 3 (Home)");
    } else if (triggeredActionButton == 11) {
      Serial.println("Action LED caused by button 11 (Notification)");
    }
  }
}

/*
 * Reset the current selection
 */
void resetSelection() {
  turnOffLED(userLED);
  turnOffLED(actionLED);
  triggeredUserButton = User::None;
  triggeredActionButton = Action::NoAction;
  userLEDState = LOW;
  actionLEDState = LOW;
}
/*
 * Blink the action LED to indicate the action cannot be taken due to some errors
 */
void indicateError() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(actionLED, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(500);                      // wait for a second
    digitalWrite(actionLED, LOW);   // turn the LED off by making the voltage LOW
    delay(500);                      // wait for a second
  }
  Serial.println("Error detected");
}

#endif
