#ifndef CLOCK_CONTROLLER_H
#define CLOCK_CONTROLLER_H

#include <RTClib.h>
#include <Stepper.h>

#define STEPS_PER_REVOLUTION 2000

Stepper hourStepper = Stepper(STEPS_PER_REVOLUTION, 4, 6, 5,7);
Stepper minuteStepper = Stepper(STEPS_PER_REVOLUTION, 8, 10, 9,11);

bool displayRealTime;

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
};

#endif



// #ifndef CLOCK_CONTROLLER_H
// #define CLOCK_CONTROLLER_H

// #include <RTClib.h>
// #include <Stepper.h>

// #define STEPS_PER_REVOLUTION 2000

// // Stepper hourStepper(STEPS_PER_REVOLUTION, 4, 6, 5, 7);
// // Stepper minuteStepper(STEPS_PER_REVOLUTION, 8, 10, 9, 11);

// Stepper hourStepper = Stepper(STEPS_PER_REVOLUTION, 4, 6, 5,7);
// Stepper minuteStepper = Stepper(STEPS_PER_REVOLUTION, 8, 10, 9,11);

// bool displayRealTime;

// struct ClockController {
//     void begin();
//     void updateClock(int hour, int minute);
//     void handleRealTimeMode();
//     void handleInputMode(String time);
//     bool isRealTimeMode() const;
//     void initClock();
//     void initializeRTC();

//     RTC_DS3231 rtc;

//     int currentHourSteps;
//     int currentMinuteSteps;
//     unsigned long switchTime;
//     int inputHour, inputMinute;
//     int currentHour, currentMinute;
//     int calculateHourSteps(int hour) const;
//     int calculateMinuteSteps(int minute) const;
//     // bool displayRealTime;
// };

// #endif
