#ifdef RUN_TESTS

#include <Arduino.h>
#include <map>
#include "Button_ISR.ino"


/* 
 * Sets up the test environment. Main test loop to simulate and verify ISR behavior.
 * Simulates button presses and checks the correctness of ISR handling,
 * LED states, and triggered events.
 * Input: None
 * Output: None
 * Toggles global variable and hardware state, Serial outputs
 */
void setup() {
  Serial.begin(9600);
  while (!Serial);

  initButtons();

  Serial.println("Starting ISR tests...");
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

  // Test 2: Trigger User Button A2
  Serial.println("\nTest 2: Trigger User Button A2");
  triggeredUserButton = None;
  digitalWrite(A2, HIGH); // Simulate button press
  delay(50);
  digitalWrite(A2, LOW); // Simulate button release
  delay(50);
  if (triggeredUserButton == User2 && userLEDState == HIGH) {
    Serial.println("Test 2 Passed: User Button A2 triggered correctly.");
  } else {
    Serial.println("Test 2 Failed: User Button A2 did not trigger correctly.");
  }

  // Test 3: Trigger User Button A3
  Serial.println("\nTest 3: Trigger User Button A2");
  triggeredUserButton = None;
  digitalWrite(A3, HIGH); // Simulate button press
  delay(50);
  digitalWrite(A3, LOW); // Simulate button release
  delay(50);
  if (triggeredUserButton == User3 && userLEDState == HIGH) {
    Serial.println("Test 3 Passed: User Button A2 triggered correctly.");
  } else {
    Serial.println("Test 3 Failed: User Button A2 did not trigger correctly.");
  }


  // Test 4: Attempt to trigger User Button A2 after User1 is selected
  Serial.println("\nTest 4: Attempt to Trigger User Button A2 after User1 is selected");
  triggeredUserButton = User1; // Set User1 as selected
  digitalWrite(A2, HIGH); // Simulate button press
  delay(50);
  digitalWrite(A2, LOW); // Simulate button release
  delay(50);
  if (triggeredUserButton == User1 && userLEDState == HIGH) { // Check if state changed
    Serial.println("Test 4 Passed: User Button A2 did not trigger as expected.");
  } else {
    Serial.println("Test 4 Failed: User Button A2 triggered incorrectly.");
  }


  // Test 5: Trigger Action Button 1 only if a user is selected
  triggeredUserButton = User1; // Pretend a user was previously selected

  Serial.println("\nTest 5: Trigger Action Button 1 after User Button A1");
  triggeredActionButton = NoAction;
  digitalWrite(actionButtons[0], HIGH); // Simulate button press
  delay(50);
  digitalWrite(actionButtons[0], LOW); // Simulate button release
  delay(50);
  if (triggeredActionButton == Action1) {
    Serial.println("Test 5 Passed: Action Button 1 triggered correctly.");
  } else {
    Serial.println("Test 5 Failed: Action Button 1 did not trigger correctly.");
  }


  // Test 6: Trigger Action Button 2 only if a user is selected
  triggeredUserButton = User1; // Pretend a user was previously selected

  Serial.println("\nTest 6: Trigger Action Button 2 after User Button A1");
  triggeredActionButton = NoAction;
  digitalWrite(actionButtons[1], HIGH); // Simulate button press
  delay(50);
  digitalWrite(actionButtons[1], LOW); // Simulate button release
  delay(50);
  if (triggeredActionButton == Action2) {
    Serial.println("Test 6 Passed: Action Button 2 triggered correctly.");
  } else {
    Serial.println("Test 6 Failed: Action Button 2 did not trigger correctly.");
  }
}

void loop() {
}

#endif
