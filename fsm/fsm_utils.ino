volatile bool actionLEDState = LOW;

/*
 * Read in button inputs and light on corresponding LEDs
 */
void updateActionButtonInputs() {
  actionLEDState = LOW; // Reset the action LED state
  // triggeredActionButton = -1;
  for (int i = 0; i < 2; i++) {
    if (digitalRead(actionButtons[i]) == HIGH) { // Button is pressed (active low)
      actionLEDState = HIGH; // Turn on the action LED
      triggeredActionButton = actionButtons[i];
      break; // Exit loop once any button is pressed
    }
  }

  digitalWrite(actionLED, actionLEDState);

  if (actionLEDState == HIGH && triggeredActionButton != -1) {
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
  lastUserButtonValues[selectedUser] = LOW;
  lastActionButtonValues[selectedAction] = LOW;
  selectedUser = user.none;
  selectedAction = action.none;
}

�%