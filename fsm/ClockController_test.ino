#ifdef RUN_TESTS

#include <RTClib.h>
#include <Stepper.h>
#include <Wire.h>

// RTC guide: https://arduinogetstarted.com/tutorials/arduino-rtc

// Initialize RTC
RTC_DS3231 rtc;

// Defines the number of steps per revolution
const int stepsPerRevolution = 2038;

// Number of steps per hour/minute for the clock hands
const int stepsPerHour = stepsPerRevolution / 12;   // 12-hour clock
const int stepsPerMinute = stepsPerRevolution / 60; // 60 minutes

// Stepper instances for hour and minute hands
Stepper hourStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper minuteStepper(stepsPerRevolution, 3, 5, 4, 6);

// Variables for tracking positions
int currentHourPos = 0;
int currentMinutePos = 0;

// State management
bool displayRealTime = true; // True for real-time, false for input time
unsigned long switchTime = 0; // Stores when to switch back to real-time
int inputHour = 0, inputMinute = 0; // Input time

/* 
 *  Initializes the test environment for clock functionality.
 * Sets up serial communication, the RTC module, and stepper motors.
 * Runs clock functionality tests if in testing mode.
 * Input: None
 * Output: None
 * Toggles global variable and hardware state
 */
void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    // Initialize the RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt if RTC is not found
    }

    // Automatically sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Initialize steppers
    hourStepper.setSpeed(10);
    minuteStepper.setSpeed(10);

    #ifdef TESTING
        runTests(); // Call to run the tests when in testing mode
    #endif
}

/* 
 *  Main loop to handle clock updates.
 * Updates the clock hands either in real-time mode using the RTC
 * or based on input time for testing purposes.
 * Input: None
 * Output: None
 * Toggles global variable and hardware state
 */
void loop() {
    // Regular operations or tests
    if (!displayRealTime && millis() > switchTime) {
        displayRealTime = true; // Revert to real-time display
    }

    if (displayRealTime) {
        DateTime now = rtc.now();
        updateClock(now.hour(), now.minute());
    }

    delay(1000); // Update every second
}

/*
 * Updates the stepper motors to display the given hour and minute.
 * 
 * Inputs:
 * hour: Target hour to display
 * minute:Target minute to display
 * Output: None
 * Side effects: Moves the stepper motors for the clock hands.
 */
void updateClock(int hour, int minute) {
    int targetHourPos = (hour % 12) * stepsPerHour;
    int targetMinutePos = minute * stepsPerMinute;

    hourStepper.step(targetHourPos - currentHourPos);
    currentHourPos = targetHourPos;

    minuteStepper.step(targetMinutePos - currentMinutePos);
    currentMinutePos = targetMinutePos;
}

/**
 * Runs a series of tests for clock functionality.
 * Includes tests to update the clock to specific times and perform
 * repeated updates to verify consistent behavior.
 * Input: None
 * Output: None
 */
void runTests() {
    Serial.println("Running clock functionality tests...");

    // Test: Update to midnight
    testUpdateTime(0, 0);

    // Test: Update to noon
    testUpdateTime(12, 0);

    // Test: Update to 11:59 PM
    testUpdateTime(23, 59);

    // Test: Repeated updates
    testRepeatedUpdates();
}

/**
 * @brief Tests updating the clock to a specific hour and minute.
 * Inputs:
 * testHour: Hour to set the clock to
 * testMinute: Minute to set the clock to
 * Outputs: None
 * Side effects: Moves the clock hands and displays the test time.
 */
void testUpdateTime(int testHour, int testMinute) {
    displayRealTime = false;
    inputHour = testHour;
    inputMinute = testMinute;

    updateClock(inputHour, inputMinute);
    Serial.print("Test: Set time to ");
    Serial.print(testHour);
    Serial.print(":");
    Serial.println(testMinute);

    delay(2000); // Display test time for 2 seconds
    displayRealTime = true; // Reset to real-time mode
}

/**
 * Tests repeated clock updates with incremental time changes.
 * Simulates updating the clock hands to several consecutive times.
 * Inputs: None
 * Output: None
 * Side effects: May move the clock hands and displays the test time.
 */
void testRepeatedUpdates() {
    for (int i = 0; i < 12; i++) {
        testUpdateTime(i, (i * 5) % 60);
        delay(1000); // Brief delay between tests
    }
}
#endif

