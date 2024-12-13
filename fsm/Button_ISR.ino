#include <map>

// States for LEDs
volatile bool userLEDState = LOW;
volatile bool actionLEDState = LOW;

// ISR for user buttons
void toggleUserLED_A1() { triggeredUserButton = User1; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A2() { triggeredUserButton = User2; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A3() { triggeredUserButton = User3; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
// void toggleUserLED_A4() { triggeredUserButton = A4; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }

void toggleActionLED() {
  if (triggeredUserButton != None) {
    for (int i = 0; i < 2; i++) {
      if (digitalRead(actionButtons[i]) == HIGH) {
        triggeredActionButton = (Action) i; // Store the button that triggered the ISR
        break;
      }
    }

    // Toggle the action LED state
    actionLEDState = HIGH;          
    digitalWrite(actionLED, actionLEDState); 
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
}