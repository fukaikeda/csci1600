#include <map>
#include "fsm.h"

// ISR for user buttons
void toggleUserLED_A1() { 
  if (triggeredUserButton == None){
    triggeredUserButton = User1; digitalWrite(userLED, HIGH);
  }
}
void toggleUserLED_A2() { 
  if (triggeredUserButton == None){
    triggeredUserButton = User2; digitalWrite(userLED, HIGH);
  }
}
void toggleUserLED_A3() {
  if (triggeredUserButton == None){
    triggeredUserButton = User3; digitalWrite(userLED, HIGH);
  }
}
// void toggleUserLED_A4() { triggeredUserButton = A4; digitalWrite(userLED, HIGH); }

void toggleActionLED() {
  if (triggeredUserButton != None) {
    for (int i = 0; i < 2; i++) {
      if (digitalRead(actionButtons[i]) == HIGH) {
        if (triggeredActionButton == NoAction){
          triggeredActionButton = (Action) i; // Store the button that triggered the ISR
          break;
        }
      }
    }

    // Toggle the action LED state
    digitalWrite(actionLED, HIGH); 
  }
}


void initButtons() {
  Serial.println("Hi setup for button ISR");

   // Initialize LEDs
  pinMode(userLED, OUTPUT);
  pinMode(actionLED, OUTPUT);
  digitalWrite(userLED, LOW); // Turn off initially
  digitalWrite(actionLED, LOW);

  attachInterrupt(digitalPinToInterrupt(A1), toggleUserLED_A1, RISING);
  attachInterrupt(digitalPinToInterrupt(A2), toggleUserLED_A2, RISING);
  attachInterrupt(digitalPinToInterrupt(A3), toggleUserLED_A3, RISING);
  // attachInterrupt(digitalPinToInterrupt(A4), toggleUserLED_A4, RISING);
  delay(1000);

  // Initialize action buttons and attach interrupts
  for (int i = 0; i < 2; i++) {
    pinMode(actionButtons[i], INPUT_PULLUP); // Set as input with pull-up
    attachInterrupt(digitalPinToInterrupt(actionButtons[i]), toggleActionLED, RISING);
  }
  // petWatchdog(); 
}