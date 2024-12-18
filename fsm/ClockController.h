#ifndef CLOCK_CONTROLLER_H
#define CLOCK_CONTROLLER_H

#include <RTClib.h>
#include <Stepper.h>
#include <Arduino.h>

#define STEPS_PER_REVOLUTION 2000

Stepper hourStepper = Stepper(STEPS_PER_REVOLUTION, 4, 6, 5,7);
Stepper minuteStepper = Stepper(STEPS_PER_REVOLUTION, 8, 10, 9,11);

/*
 * ClockController structure to manage clock functionality.
 * Provides methods for initializing the clock, handling real-time and input
 * modes, and updating stepper motor positions to reflect the desired time.
 */
struct ClockController {
    void begin();
    void updateClock(int hour, int minute);
    void handleRealTimeMode();
    int handleInputMode(String time);
    bool isRealTimeMode() const;
    void initClock();
    void initializeRTC();

    RTC_DS3231 rtc;

    int currentHourSteps;
    int currentMinuteSteps;
    unsigned long switchTime;
    int inputHour, inputMinute;
    int currentHour, currentMinute;
    int calculateHourSteps(int hour) const;
    int calculateMinuteSteps(int minute) const;
    bool displayRealTime;
};

#endif
