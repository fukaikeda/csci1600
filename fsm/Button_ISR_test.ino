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

void loop() {
}

#endif
