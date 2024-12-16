#include <map>

const int userButtons[4] = {A1, A2, A3, A5}; // User buttons Do not change!!
const int actionButtons[2] = {2, 3};       // Action buttons
const int userLED = 12;                      // LED for user buttons
const int actionLED = 13;                    // LED for action buttons

// States for LEDs
volatile bool userLEDState = LOW;
volatile bool actionLEDState = LOW;

// Include additional variables to store the triggered button
volatile int triggeredUserButton = -1; // Variable to track the user button that triggered ISR
volatile int triggeredActionButton = -1; // Variable to track the action button that triggered ISR

std::map<String, String> homeTimes = {
    {"Mia", ""},
    {"Fuka", ""},
    {"Jason", ""},
    {"Kana", "test"}
};


// ISR for user buttons
void toggleUserLED_A1() { triggeredUserButton = A1; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A2() { triggeredUserButton = A2; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
void toggleUserLED_A3() { triggeredUserButton = A3; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }
// void toggleUserLED_A4() { triggeredUserButton = A4; userLEDState = HIGH; digitalWrite(userLED, userLEDState); }


// ISR for action buttons
void toggleActionLED() {
  // Identify the button that triggered the ISR
  for (int i = 0; i < 2; i++) {
    if (digitalRead(actionButtons[i]) == HIGH) {
      triggeredActionButton = actionButtons[i]; // Store the pin of the triggered button
      break;
    }
  }

  // Toggle the action LED state
  actionLEDState = HIGH;          
  digitalWrite(actionLED, actionLEDState); 
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for Serial to initialize

  Serial.println("Hi setup");
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

void loop() {
  // Print which button triggered the ISR if any
  Serial.println("print");
  if (triggeredUserButton != -1) {
    Serial.print("User button triggered ISR: ");
    Serial.println(triggeredUserButton);
    // triggeredUserButton = -1; // Reset after processing
  }

  if (triggeredActionButton != -1) {
    Serial.print("Action button triggered ISR: ");
    Serial.println(triggeredActionButton);
    // triggeredActionButton = -1; // Reset after processing
  }

  triggeredUserButton = -1;
  triggeredActionButton = -1;

  // Placeholder for other logic
  delay(1000); // Adjust delay as needed
  resetButtons();
}


void resetButtons() {
  // if (userLEDState) {
    userLEDState = LOW;             // Turn off user LED
    digitalWrite(userLED, userLEDState);
  // }
  // if (actionLEDState) {
    actionLEDState = LOW;           // Turn off action LED
    digitalWrite(actionLED, actionLEDState);
  // }

  // Reset last button value
}
