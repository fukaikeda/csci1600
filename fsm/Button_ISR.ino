#include <map>

// States for LEDs
volatile bool userLEDState = LOW;

// ISR for user buttons
void toggleUserLED_A1() { triggeredUserButton = A1; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A2() { triggeredUserButton = A2; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A3() { triggeredUserButton = A3; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
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


void loop() {
  // Print which button triggered the ISR if any
  Serial.println("print");
  if (triggeredUserButton != -1) {
    Serial.print("User button triggered ISR: ");
    Serial.println(triggeredUserButton);
    // triggeredUserButton = -1; // Reset after processing
  }

  // if (triggeredActionButton != -1) {
  //   Serial.print("Action button triggered ISR: ");
  //   Serial.println(triggeredActionButton);
  //   // triggeredActionButton = -1; // Reset after processing
  // }

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

  

  // if (triggeredUserButton == 18 && triggeredActionButton == 3) {
  //   Serial.println("Kana Home: ");
  //   String key = "Kana";
  //   // Serial.print(key + ": ");
  //   Serial.println(homeTimes[key]);
  // }
  // if (triggeredUserButton == 17 && triggeredActionButton == 3) {
  //   Serial.println("Jason Home: ");
  //   String key = "Jason";
  //   // Serial.print(key + ": ");
  //   Serial.println(homeTimes[key]);
  // }
  // if (triggeredUserButton == 16 && triggeredActionButton == 3) {
  //   Serial.println("Fuka Home: ");
  //   String key = "Fuka";
  //   // Serial.print(key + ": ");
  //   Serial.println(homeTimes[key]);
    
  // }
  // if (triggeredUserButton == 15 && triggeredActionButton == 3) {
  //   Serial.println("Mia Home");
  //   String key = "Mia";
  //   // Serial.print(key + ": ");
  //   Serial.println(homeTimes[key]);
  // }
  

  // if (triggeredUserButton == 18 && triggeredActionButton == 11) {
  //   Serial.println("Kana notification");
  // }
  // if (triggeredUserButton == 17 && triggeredActionButton == 11) {
  //   Serial.println("Jason notification");
  // }
  // if (triggeredUserButton == 16 && triggeredActionButton == 11) {
  //   Serial.println("Fuka notification");
  // }
  // if (triggeredUserButton == 15 && triggeredActionButton == 11) {
  //   Serial.println("Mia notification");
  // }

  triggeredUserButton = -1;
  // triggeredActionButton = -1;

  // Placeholder for other logic
  delay(100); // Adjust delay as needed
}



// const int userButtons[4] = {A1, A2, A3, A4}; // User buttons Do not change!!
// const int actionButtons[2] = {3, 11};         // Action buttons 3, 11; 3,6 ; 
// const int userLED = 7;                      // LED for user buttons
// const int actionLED = 8;                    // LED for action buttons

// // States for LEDs
// volatile bool userLEDState = LOW;
// volatile bool actionLEDState = LOW;

// // State tracking for button presses
// int lastUserButtonValues[4] = {LOW, LOW, LOW, LOW};
// int lastActionButtonValues[2] = {LOW, LOW};

// // ISR for user buttons
// // void toggleUserLED() {
// //   userLEDState = HIGH;            
// //   digitalWrite(userLED, userLEDState);
// //   for (int i = 0; i < 4; i++) {
// //     if (digitalRead(userButtons[i]) == HIGH) {
// //       lastUserButtonValues[i] = HIGH;
// //       break;
// //     }
// //   }
// // }


// // void toggleUserLED() {
// //   // Toggle the user LED state
// //   userLEDState = !userLEDState;           
// //   digitalWrite(userLED, userLEDState); 
// //   // for (int i = 0; i < 4; i++) {
// //   //   if (digitalRead(userButtons[i]) == HIGH) {
// //   //     lastUserButtonValues[i] = HIGH;
// //   //     break;
// //   //   }
// //   // }
// // }

// const unsigned long debounceDelay = 50; 
// volatile unsigned long lastInterruptTime[6] = {0, 0, 0, 0, 0, 0};

// // ISR for user buttons
// void toggleUserLED() {
//   unsigned long currentTime = millis();
//   int buttonIndex = -1;

//   // Determine which button triggered the interrupt
//   for (int i = 0; i < 4; i++) {
//     if (digitalRead(userButtons[i]) == HIGH) {
//       buttonIndex = i;
//       break;
//     }
//   }

//   // Check debounce
//   if (buttonIndex != -1 && (currentTime - lastInterruptTime[buttonIndex]) > debounceDelay) {
//     lastUserButtonValues[buttonIndex] = HIGH;
//     lastInterruptTime[buttonIndex] = currentTime;
//   }

//   // Toggle the user LED
//   userLEDState = !userLEDState;
//   digitalWrite(userLED, userLEDState);
// }


// // ISR for action buttons
// void toggleActionLED() {
//   actionLEDState = !actionLEDState;          
//   digitalWrite(actionLED, actionLEDState); 
//   // for (int i = 0; i < 2; i++) {
//   //   if (digitalRead(actionButtons[i]) == HIGH) {
//   //     lastActionButtonValues[i] = HIGH;
//   //     break;
//   //   }
//   // }

// }


// void resetButtons() {
//   // if (userLEDState) {
//     userLEDState = LOW;             // Turn off user LED
//     digitalWrite(userLED, userLEDState);
//   // }
//   // if (actionLEDState) {
//     actionLEDState = LOW;           // Turn off action LED
//     digitalWrite(actionLED, actionLEDState);
//   // }

//   // Reset last button values
//   for (int i = 0; i < 4; i++) {
//     lastUserButtonValues[i] = LOW;
//   }
//   for (int i = 0; i < 2; i++) {
//     lastActionButtonValues[i] = LOW;
//   }
// }

// void setup() {
//   Serial.begin(9600);
//   while (!Serial);                // Wait for Serial to initialize

//   Serial.println("Hi setup");
//   // Initialize LEDs
//   pinMode(userLED, OUTPUT);
//   pinMode(actionLED, OUTPUT);
//   digitalWrite(userLED, LOW);     // Turn off initially
//   digitalWrite(actionLED, LOW);

//   // Initialize user buttons and attach interrupts
//   for (int i = 0; i < 4; i++) {
//     pinMode(userButtons[i], INPUT_PULLUP); // Set as input with pull-up
//     attachInterrupt(digitalPinToInterrupt(userButtons[i]), toggleUserLED, RISING);
//   }
//   delay(1000);

//   // Initialize action buttons and attach interrupts
//   for (int i = 0; i < 2; i++) {
//     pinMode(actionButtons[i], INPUT_PULLUP); // Set as input with pull-up
//     attachInterrupt(digitalPinToInterrupt(actionButtons[i]), toggleActionLED, RISING);
//   }
// }

// void loop() {
//   Serial.println("Hi loop");


//   // Debug output
//   Serial.print("User Button States: ");
//   for (int i = 0; i < 4; i++) {
//     Serial.print(lastUserButtonValues[i]);
//     Serial.print(" ");
//   }
//   Serial.println();

//   Serial.print("Action Button States: ");
//   for (int i = 0; i < 2; i++) {
//     Serial.print(lastActionButtonValues[i]);
//     Serial.print(" ");
//   }
//   Serial.println();

//   delay(5000); // Adjust delay as needed
//   // resetButtons();

  
// }
