#ifndef MYWDT_H
#define MYWDT_H

#include <WDT.h>
#include <Arduino.h>
/*
Watchdog Refresh


This sketch shows how to enable the watchdog and
refresh the timer to avoid resets


Watchdog intervals are limited to 7 timeout periods
the library will select the best clock divisor and timeout
according to the selected wdtInterval.


UNO R4 min wdtInterval 1ms / max wdtInterval 5592ms
Comment out Serial.print() in the setup to make it work with
small intervals


Portenta C33 min wdtInterval 1ms / max wdtInterval 2684ms


Circuit:
- Portenta C33
- UNO R4
*/
const long wdtInterval = 2684;

void setupWDT();
void petWatchdog();

#endif 