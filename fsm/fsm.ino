#include "fsm.h"

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize
  // set up pin modes
  pinMode(userButtons[0], INPUT);
  pinMode(userButtons[1], INPUT);
  pinMode(userButtons[2], INPUT);
  pinMode(userButtons[3], INPUT);
  pinMode(returnTimeButton, INPUT);
  pinMode(messageButton, INPUT);
  pinMode(userLEDs[0], OUTPUT);
  pinMode(userLEDs[1], OUTPUT);
  pinMode(userLEDs[2], OUTPUT);
  pinMode(userLEDs[3], OUTPUT);
  pinMode(returnTimeLED, OUTPUT);
  pinMode(messageLED, OUTPUT);
}

void loop() {
  updateInputs();
  // CURRENT_STATE = updateFSM(CURRENT_STATE, millis(), numButtonsPressed, lastButtonPressed);
  delay(10);
}

state updateFSM(state curState, long mils) {
  
}
