volatile bool actionLEDState = LOW;

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

void turnOnLED(int ledPin) {
  digitalWrite(ledPin, HIGH);
}

void turnOffLED(int ledPin) {
  digitalWrite(ledPin, LOW);
}

/*
 * Reset the current selection
 */
void resetSelection() {
  turnOffLED(userLED);
  turnOffLED(actionLED);
  triggeredUserButton = User::None;
  triggeredActionButton = Action::NoAction;
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