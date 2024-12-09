/*
 * Read in button inputs and light on corresponding LEDs
 */
void updateInputs() {
  // User button inputs
  if (selectedUser == user.none) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(userButtons[i]) == HIGH && lastUserButtonValues[i] == LOW) {
        //digitalWrite(userLEDs[i], HIGH);
        lastUserButtonValues[i] = HIGH;
        selectedUser = i;
        break;  // prioritize users with smaller numbers for selection
      }
    }
  }

  if (selectedUser != user.none && selectedAction == action.none) {
    // Action button inputs
    for (int i = 0; i < 2; i++) {
      if (digitalRead(actionButtons[i]) == HIGH && lastActionButtonValues[i] == LOW) {
        //digitalWrite(actionLEDs[i], HIGH);
        lastActionButtonValues[i] = HIGH;
        selectedAction = i;
        break;  // prioritize action with a smaller number for selection
      }
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

