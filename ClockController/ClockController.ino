#include "ClockController.h"
#include <Arduino.h>

ClockController::ClockController(int hourPin, int minutePin)
    : currentHourAngle(0), currentMinuteAngle(0), switchTime(0), displayRealTime(true), inputHour(0), inputMinute(0) {
    hourServo.attach(hourPin);
    minuteServo.attach(minutePin);
}

// Method for intializing clock 

void ClockController::initClock() {
    Serial.begin(9600);

    // Initialize RTC
    initializeRTC();

    // Callibration of clock through terminal input
    Serial.println("Enter current clock time as HHMM (e.g., 1030 for 10:30):");
    while (!Serial.available());
    String initialTime = Serial.readStringUntil('\n');
    int initHour = initialTime.substring(0, 2).toInt();
    int initMinute = initialTime.substring(2, 4).toInt();

    // Initialize positions to current time 
    currentHourAngle = calculateHourAngle(initHour);
    currentMinuteAngle = calculateMinuteAngle(initMinute);
    hourServo.write(currentHourAngle);
    minuteServo.write(currentMinuteAngle);

    // Status message for completing calibration steps 
    Serial.println("Initialization complete.");
    Serial.print("Hour angle: "); Serial.println(currentHourAngle);
    Serial.print("Minute angle: "); Serial.println(currentMinuteAngle);
}

// Method for intializing RTC (real-time clock module) 

void ClockController::initializeRTC() {

    // Error handling for RTC not found
    if (!rtc.begin()) {
        Serial.println("ERROR: Couldn't find RTC. Check connections.");
        while (1);
    }

    // Initilization state in first setup or when power is lost
    if (rtc.lostPower()) {
        Serial.println("RTC lost power. Setting to compile time.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

// Method for handling real time on the clock 

void ClockController::handleRealTimeMode() {
    if (displayRealTime) {

        // Get real time from rtc 
        DateTime now = rtc.now();

        // Serial output for ease and debugging
        Serial.print("Real-time mode. Current RTC time: ");
        Serial.print(now.hour(), DEC);
        Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.println(now.minute(), DEC);

        // Tells clock to update 
        updateClock(now.hour(), now.minute());
    } else if (millis() > switchTime) {
        displayRealTime = true;
        Serial.println("Switching back to real-time mode.");
    }
}

// Method for handling time input 

void ClockController::handleInputMode(int time) {

    // Checks length of input 
    if (time.length() == 4) {

        // Parses input to hours and minutes 
        inputHour = time.substring(0, 2).toInt();
        inputMinute = time.substring(2, 4).toInt();

        // Checks if time numbers are valid 
        if (inputHour < 24 && inputMinute < 60) {
            displayRealTime = false;
            switchTime = millis() + 10000; // Checks if user time display times out

            // Serial output of time for ease 
            Serial.print("Input time received: ");
            Serial.print(inputHour); Serial.print(":");
            Serial.println(inputMinute);

            // Sends motor commands
            updateClock(inputHour, inputMinute);

            // Error handling for invalid time input 
            else {
                Serial.println("ERROR: Invalid time input. Use format HHMM.");
            }
        } 
        else {
            Serial.println("ERROR: Input must be 4 digits (HHMM).");
        }
    }
}

// If clock is in real time mode, it should display the real time

bool ClockController::isRealTimeMode() const {
    return displayRealTime;
}

// Function for changing software comamnds to motor movements

void ClockController::updateClock(int hour, int minute) {
    
    // Calculate target angles
    int targetHourAngle = calculateHourAngle(hour);
    int targetMinuteAngle = calculateMinuteAngle(minute);

    // Serial output for debugging 
    Serial.print("Updating clock to ");
    Serial.print(hour); Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.println(minute);

    // Smooth transition for hour servo
    for (int pos = currentHourAngle; pos != targetHourAngle; pos += (targetHourAngle > currentHourAngle) ? 1 : -1) {
        hourServo.write(pos);
        delay(10); // Delay added for smooth transition
    }

    // Stores current hour 
    currentHourAngle = targetHourAngle;

    // Smooth transition for minute servo
    for (int pos = currentMinuteAngle; pos != targetMinuteAngle; pos += (targetMinuteAngle > currentMinuteAngle) ? 1 : -1) {
        minuteServo.write(pos);
        delay(5); // Delay added for smooth transition
    }

    // Stores current minute 
    currentMinuteAngle = targetMinuteAngle;

    Serial.println("Clock update complete.");
}

// Helper function for calculating hour angles

int ClockController::calculateHourAngle(int hour) const {
    return map(hour % 12, 0, 12, 0, 180);
}

// Helper function for calculating minute angles

int ClockController::calculateMinuteAngle(int minute) const {
    return map(minute, 0, 60, 0, 180);
}
