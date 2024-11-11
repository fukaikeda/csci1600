#include <Stepper.h>

// Defines the number of steps per revolution
const int stepsPerRevolution = 2038;

// Number of steps per hour/minute for the clock hands
const int stepsPerHour = stepsPerRevolution / 12; // Assuming 12-hour clock for hours
const int stepsPerMinute = stepsPerRevolution / 60; // 60 minutes in a full rotation

// Creates an instance of stepper class
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11); // Hour hand
Stepper myStepper2 = Stepper(stepsPerRevolution, 3, 5, 4, 6);   // Minute hand

// Variables to store the last known positions of each motor
int currentHourPos = 0;
int currentMinutePos = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    myStepper.setSpeed(10);  // Set speed for hour hand
    myStepper2.setSpeed(10); // Set speed for minute hand
}

void loop() {
    // Check if serial input is available
    if (Serial.available() >= 4) {  // We expect 4 characters for "HHMM"
        // Read and parse time input
        String timeInput = Serial.readStringUntil('\n');
        int hour = timeInput.substring(0, 2).toInt();   // Get the hour value
        int minute = timeInput.substring(2, 4).toInt(); // Get the minute value

        // Calculate the target step positions
        int targetHourPos = (hour % 12) * stepsPerHour; // Map hour to 12-hour position
        int targetMinutePos = minute * stepsPerMinute;  // Map minute to step position

        // Move hour hand to the target position
        int hourStepsToMove = targetHourPos - currentHourPos;
        myStepper.step(hourStepsToMove);
        currentHourPos = targetHourPos; // Update current hour position

        // Move minute hand to the target position
        int minuteStepsToMove = targetMinutePos - currentMinutePos;
        myStepper2.step(minuteStepsToMove);
        currentMinutePos = targetMinutePos; // Update current minute position

        // Add a delay before reading the next input
        delay(1000);
    }
}

