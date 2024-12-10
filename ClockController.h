#ifndef CLOCK_CONTROLLER_H
#define CLOCK_CONTROLLER_H

#include <RTClib.h>
#include <Servo.h>

class ClockController {
public:
    ClockController(int hourPin, int minutePin);
    void begin();
    void updateClock(int hour, int minute);
    void handleRealTimeMode();
    void handleInputMode();
    bool isRealTimeMode() const;

private:
    RTC_DS3231 rtc;
    Servo hourServo;
    Servo minuteServo;

    int currentHourAngle;
    int currentMinuteAngle;
    unsigned long switchTime;
    bool displayRealTime;
    int inputHour, inputMinute;

    void initializeRTC();
    int calculateHourAngle(int hour) const;
    int calculateMinuteAngle(int minute) const;
};

#endif
