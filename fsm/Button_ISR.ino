#include <map>

// States for LEDs
volatile bool userLEDState = LOW;

// ISR for user buttons
void toggleUserLED_A1() { triggeredUserButton = User1; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A2() { triggeredUserButton = User2; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A3() { triggeredUserButton = User3; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
// void toggleUserLED_A4() { triggeredUserButton = A4; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }


void initButtons() {
  Serial.println("Hi setup for button ISR");
  // Initialize LEDs
  pinMode(userLED, OUTPUT);
  pinMode(actionLED, OUTPUT);
  // Turn off initially
  digitalWrite(userLED, LOW); 
  digitalWrite(actionLED, LOW);

  // attach ISR
  attachInterrupt(digitalPinToInterrupt(A1), toggleUserLED_A1, RISING);
  attachInterrupt(digitalPinToInterrupt(A2), toggleUserLED_A2, RISING);
  attachInterrupt(digitalPinToInterrupt(A3), toggleUserLED_A3, RISING);
  // attachInterrupt(digitalPinToInterrupt(A4), toggleUserLED_A4, RISING);
  
  for (int i = 0; i < 2; i++) {
    pinMode(actionButtons[i], INPUT_PULLUP); // Set as input with pull-up
  }
}