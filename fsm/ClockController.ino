#include "ClockController.h"

// Method for intializing clock 

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

// Method for intializing RTC (real-time clock module) 

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

// Method for handling real time on the clock 

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

// Method for handling time input 

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


// If clock is in real time mode, it should display the real time

bool ClockController::isRealTimeMode() const {
    return displayRealTime;
}

// Function for changing software comamnds to motor movements

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


// Helper function for calculating hour steps

int ClockController::calculateHourSteps(int hour) const {
    return map((12 - (hour % 12)), 0, 12, 0, STEPS_PER_REVOLUTION);
}

// Helper function for calculating minute steps

int ClockController::calculateMinuteSteps(int minute) const {
    return map(minute, 0, 60, 0, STEPS_PER_REVOLUTION);
}


