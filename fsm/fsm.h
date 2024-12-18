#ifndef FSM_H
#define FSM_H

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

/*
 * User enumeration for button inputs.
 * Represents the users who can trigger inputs through buttons.
 */
typedef enum {
  User1 = 0,
  User2 = 1,
  User3 = 2,
  User4 = 3,
  None = -1
} User;

/*
 * Action enumeration for button-triggered events.
 * Represents the actions that can be taken via the action buttons.
 */
typedef enum {
  ReturnTime = 0,
  Message = 1,
  NoAction = -1
} Action;

/**
 * FSM State enumeration for tracking the current state.
 * Represents the various states in the Finite State Machine.
 */
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

/*
 * Turns off an LED connected to the specified pin.
 * Inputs: ldPin The pin connected to the LED.
 * Outputs; None
 * Side effects: The LED is turned off.
 */
void turnOnLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
}

void turnOffLED(int ledPin) {
  digitalWrite(ledPin, LOW);
}

/*
 * Resets the current selection of user and action inputs.
 * Turns off both the user and action LEDs and clears any button triggers.
 * Inputs: NOne
 * Outputs: None
 * Side effects: LEDs are turned off, and trigger variables are reset.
 */
void resetSelection() {
  turnOffLED(userLED);
  turnOffLED(actionLED);
  triggeredUserButton = User::None;
  triggeredActionButton = Action::NoAction;
}
/*
 * Blink the action LED to indicate the action cannot be taken due to some errors
 * Inputs: None
 * Outputs: None
 * Side effects: The action LED blinks, and an error message is printed to serial.
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
