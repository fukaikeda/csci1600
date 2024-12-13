#ifdef TESTING

#include <Arduino.h>
#include <map>

// Mock pin definitions
const int userLED = 12;        // User LED
const int actionLED = 13;      // Action LED
const int userButtons[3] = {A1, A2, A3}; // User buttons
const int actionButtons[2] = {2, 3};     // Action buttons

// Mock enums
enum User { None = -1, User1, User2, User3 };
enum Action { NoAction = -1, Action1, Action2 };

// Global variables
volatile User triggeredUserButton = None;
volatile Action triggeredActionButton = NoAction;

// States for LEDs
volatile bool userLEDState = LOW;
volatile bool actionLEDState = LOW;

// ISR Handlers (from the provided code)
void toggleUserLED_A1() { triggeredUserButton = User1; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A2() { triggeredUserButton = User2; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A3() { triggeredUserButton = User3; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }

void toggleActionLED() {
  if (triggeredUserButton != None) {
    for (int i = 0; i < 2; i++) {
      if (digitalRead(actionButtons[i]) == HIGH) {
        triggeredActionButton = (Action) i; // Store the button that triggered the ISR
        break;
      }
    }
    actionLEDState = HIGH;
    digitalWrite(actionLED, actionLEDState);
  }
}

// Initialization function (from the provided code)
void initButtons() {
  Serial.println("Initializing button ISR testing...");

  pinMode(userLED, OUTPUT);
  pinMode(actionLED, OUTPUT);
  digitalWrite(userLED, LOW);
  digitalWrite(actionLED, LOW);

  attachInterrupt(digitalPinToInterrupt(A1), toggleUserLED_A1, RISING);
  attachInterrupt(digitalPinToInterrupt(A2), toggleUserLED_A2, RISING);
  attachInterrupt(digitalPinToInterrupt(A3), toggleUserLED_A3, RISING);

  for (int i = 0; i < 2; i++) {
    pinMode(actionButtons[i], INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(actionButtons[i]), toggleActionLED, RISING);
  }
  delay(1000);
}

// Test setup function
void setup() {
  Serial.begin(9600);
  while (!Serial);

  initButtons();

  Serial.println("Starting ISR tests...");
}

// Test loop function
void loop() {
  // Test 1: Trigger User Button A1
  Serial.println("\nTest 1: Trigger User Button A1");
  triggeredUserButton = None;
  digitalWrite(A1, HIGH); // Simulate button press
  delay(50);
  digitalWrite(A1, LOW); // Simulate button release
  delay(50);
  if (triggeredUserButton == User1 && userLEDState == HIGH) {
    Serial.println("Test 1 Passed: User Button A1 triggered correctly.");
  } else {
    Serial.println("Test 1 Failed: User Button A1 did not trigger correctly.");
  }

  // Test 2: Trigger Action Button 2
  Serial.println("\nTest 2: Trigger Action Button 2 after User Button A1");
  triggeredActionButton = NoAction;
  digitalWrite(2, HIGH); // Simulate button press
  delay(50);
  digitalWrite(2, LOW); // Simulate button release
  delay(50);
  if (triggeredActionButton == Action1 && actionLEDState == HIGH) {
    Serial.println("Test 2 Passed: Action Button 2 triggered correctly.");
  } else {
    Serial.println("Test 2 Failed: Action Button 2 did not trigger correctly.");
  }

  // Test 3: No User Button Selected, Trigger Action Button
  Serial.println("\nTest 3: Trigger Action Button with No User Button Selected");
  triggeredUserButton = None;
  triggeredActionButton = NoAction;
  actionLEDState = LOW;
  digitalWrite(3, HIGH); // Simulate button press
  delay(50);
  digitalWrite(3, LOW); // Simulate button release
  delay(50);
  if (actionLEDState == LOW) {
    Serial.println("Test 3 Passed: Action Button did not trigger without User Button selected.");
  } else {
    Serial.println("Test 3 Failed: Action Button triggered without User Button selected.");
  }

  delay(5000); // Wait before restarting the loop
}

#endif
