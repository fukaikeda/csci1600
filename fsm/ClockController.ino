#include "ClockController.h"

/* 
 * Initializes the clock controller and calibrates initial time.
 * Initializes internal variables, sets up the RTC, and prompts the user for initial
 * clock calibration time via the terminal.
 * Input: None
 * Output: None
 * Toggles motor hardware to calibrate
 */
void ClockController::initClock() {
  
    // Serial.begin(9600);
    petWatchdog(); 
    currentHourSteps = 0;
    currentMinuteSteps = 0;
    switchTime = 0;
    displayRealTime = true;
    inputHour = 0;
    inputMinute = 0;

    // Initialize RTC
    initializeRTC();

    // Callibration of clock through terminal input
    Serial.println("Enter current clock time as HHMM (e.g., 1030 for 10:30):");
    // while (!Serial.available());
    while (!Serial.available()) {
      // petWatchdog();  // Reset watchdog while waiting for input
      // delay(50);      // Include some delay to reduce CPU usage
    }
    String initialTime = Serial.readStringUntil('\n');
    int initHour = initialTime.substring(0, 2).toInt();
    int initMinute = initialTime.substring(2, 4).toInt();

    // Initialize positions to current time 
    currentHourSteps = calculateHourSteps(initHour);
    currentMinuteSteps = calculateMinuteSteps(initMinute);
    hourStepper.step(currentHourSteps);
    minuteStepper.step(currentMinuteSteps);

    // Status message for completing calibration steps 
    Serial.println("Initialization complete.");
    Serial.print("Hour steps: "); Serial.println(currentHourSteps);
    Serial.print("Minute steps: "); Serial.println(currentMinuteSteps);
    // petWatchdog(); 
    
}

/*
 * Initializes the RTC module.
 * Ensures the RTC is connected and operational. If the RTC lost power, it resets the time
 * to the current compile-time.
 * Inputs: None
 * Outputs: None
 * Side effects: RTC time may be adjusted if power loss is detected.
 */
void ClockController::initializeRTC() {
    petWatchdog(); 
    // Error handling for RTC not found
    if (!rtc.begin()) {
        Serial.println("ERROR: Couldn't find RTC. Check connections.");
        while (1);
    }
    petWatchdog(); 
    // Initilization state in first setup or when power is lost
    if (rtc.lostPower()) {
        Serial.println("RTC lost power. Setting to compile time.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    petWatchdog(); 
}

/*
 * Handles real-time clock mode.
 * Reads the current time from the RTC and updates the clock hands accordingly. If the
 * display mode is not real-time and a timeout occurs, it switches back to real-time mode.
 * Inputs: None
 * Outputs: None
 * Side effects: Updates stepper motors to reflect the current time.
 */
void ClockController::handleRealTimeMode() {
    // Serial.println("not FSM, displayRealTime: ");
    // Serial.println(displayRealTime);
    petWatchdog(); 
    if (displayRealTime) {
        petWatchdog(); 

        // Get real time from rtc 
        DateTime now = rtc.now();

        // Serial output for ease and debugging
        Serial.print("Real-time mode. Current RTC time: ");
        Serial.print(now.hour(), DEC);
        Serial.print(":");
        if (now.minute() < 10) Serial.print("0");
        Serial.println(now.minute(), DEC);

        // Tells clock to update 
        petWatchdog(); 
        updateClock(now.hour(), now.minute());
        petWatchdog(); 
    } else if (millis() > switchTime) {
        petWatchdog(); 
        displayRealTime = true;
        Serial.println("Switching back to real-time mode.");
        petWatchdog(); 
    }
    petWatchdog(); 
    delay(50);
}

/*
 * @Handles manual time input and updates the clock.
 * Parses user input for time in "HHMM" format, validates it, and moves the clock hands
 * to the specified time. It also sets a timeout to revert to real-time mode.
 * InputsL
 * time: Input time string
 * Outputs: int 
 * Returns 0 on success, -1 on invalid input
 * Side effects: Updates stepper motor positions based on user input.
 */
int ClockController::handleInputMode(String time) {
    petWatchdog();
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
            petWatchdog();
        }
        // Error handling for invalid time input 
        else {
            Serial.println("ERROR: Invalid time input. Use format HHMM.");
            // petWatchdog();
            return -1;
        }
      // petWatchdog();
    } 
    else {
        Serial.println("ERROR: Input must be 4 digits (HHMM).");
        // petWatchdog();
        return -1;
    }
    petWatchdog();
    return 0;
}


/**
 * Checks if the clock is in real-time mode.
 * Inputs: None
 * Outputs: true if in real-time mode, false otherwise.
 */
bool ClockController::isRealTimeMode() const {
    return displayRealTime;
}

/*
 * Updates the stepper motors to reflect the specified time.
 * Inputs:
 * hour: Target hour to display
 * minute: Target minute to display
 * Outputs: None
 * Side effects: Moves the hour and minute stepper motors to the correct position.
 */
void ClockController::updateClock(int hour, int minute) {
    petWatchdog();
    const int stepsPerRevolution = 2038;

    // Calculate target steps
    int targetHourSteps = calculateHourSteps(hour);
    int targetMinuteSteps = calculateMinuteSteps(minute);
    petWatchdog();
    // Check if the target hour and minute are the same as the current ones
    if (hour == currentHour && minute == currentMinute) {
        // Serial.println("No update needed; time hasn't changed.");
        petWatchdog();
        return;
    }

    // Update current hour and minute
    currentHour = hour;
    currentMinute = minute;

    // Serial output for debugging
    Serial.print("Updating clock to ");
    Serial.print(hour); Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.println(minute);

    // hour stepper， need to petWatchdog since moving motor takes time
    petWatchdog();
    hourStepper.setSpeed(10);
    petWatchdog();
    
    hourStepper.step(targetHourSteps - currentHourSteps);
    petWatchdog();
    currentHourSteps = targetHourSteps;

    // minute stepper
    minuteStepper.setSpeed(10);
    petWatchdog();
    minuteStepper.step(targetMinuteSteps - currentMinuteSteps);
    petWatchdog();
    currentMinuteSteps = targetMinuteSteps;

    Serial.println("Clock update complete.");
    petWatchdog();
}


/**
 * Calculates the stepper position for a given hour.
 * Inputs: 
 * hour: the hour to calculate steps for
 * Ourputs: int Step position for the hour hand
 */
int ClockController::calculateHourSteps(int hour) const {
    return map((12 - (hour % 12)), 0, 12, 0, STEPS_PER_REVOLUTION);
}

/*
 * Calculates the stepper position for a given minute.
 * Inputs:
 * minute: The minute to calculate steps for (0-59)
 * Outputs: int Step position for the minute hand
 */
int ClockController::calculateMinuteSteps(int minute) const {
    return map(minute, 0, 60, 0, STEPS_PER_REVOLUTION);
}


