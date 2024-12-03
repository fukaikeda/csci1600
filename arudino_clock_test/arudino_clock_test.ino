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
Stepper hourStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper minuteStepper = Stepper(stepsPerRevolution, 3, 5, 4, 6);

// Variables for tracking positions
int currentHourPos = 0;
int currentMinutePos = 0;

// State management
bool displayRealTime = true; // True for real-time, false for input time
unsigned long switchTime = 0; // Stores when to switch back to real-time
int inputHour = 0, inputMinute = 0; // Input time

void setup() {
    Serial.begin(9600);

    // Initialize the RTC
    if (!rtc.begin()) {
        Serial.println("Couldn't find RTC");
        while (1); // Halt if RTC is not found
    }
    
    // Automatically sets the RTC to the date & time on PC this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Initialize steppers
    hourStepper.setSpeed(10);
    minuteStepper.setSpeed(10);
}

void loop() {
    // Print mode and current time to Serial
    if (displayRealTime) {
        DateTime now = rtc.now(); // Get real-time clock value
        Serial.print("Mode: Real-time, Time: ");
        Serial.print(now.hour(), DEC);
        Serial.print(":");
        if (now.minute() < 10) {
            Serial.print("0"); // Leading zero for minutes < 10
        }
        Serial.println(now.minute(), DEC);
    } else {
        Serial.print("Mode: Input mode, Time: ");
        Serial.print(inputHour, DEC);
        Serial.print(":");
        if (inputMinute < 10) {
            Serial.print("0"); // Leading zero for minutes < 10
        }
        Serial.println(inputMinute, DEC);
    }

    // Only accept serial input when in real-time mode (displayRealTime == true)
    if (displayRealTime && Serial.available() >= 4) {
        // Read military time input
        String timeInput = Serial.readStringUntil('\n');
        inputHour = timeInput.substring(0, 2).toInt();   // Get hour
        inputMinute = timeInput.substring(2, 4).toInt(); // Get minute

        // Set to display input time and start 10-second timer
        displayRealTime = false;
        switchTime = millis() + 10000; // 10 seconds from now

        updateClock(inputHour, inputMinute); // Update to input time
    }

    // Check if it's time to switch back to real-time
    if (!displayRealTime && millis() > switchTime) {
        displayRealTime = true; // Revert to real-time display
    }

    // Update clock based on current mode
    if (displayRealTime) {
        DateTime now = rtc.now(); // Get real-time clock value
        updateClock(now.hour(), now.minute());
    }

    delay(1000); // Update every second
}

// Function to update the clock hands
void updateClock(int hour, int minute) {
    // Calculate target step positions
    int targetHourPos = (hour % 12) * stepsPerHour;
    int targetMinutePos = minute * stepsPerMinute;

    // Move hour hand
    int hourStepsToMove = targetHourPos - currentHourPos;
    hourStepper.step(hourStepsToMove);
    currentHourPos = targetHourPos;

    // Move minute hand
    // Calculate the raw difference
    int minuteStepsToMove = targetMinutePos - currentMinutePos;

    // Move the stepper motor in the opposite direction
    minuteStepper.step(-minuteStepsToMove);

    // Update the current position
    currentMinutePos = targetMinutePos;
}
