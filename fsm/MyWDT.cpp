#include "MyWDT.h"

unsigned long wdtMil = 0;

/*
 * Initializes the watchdog timer with a specified interval.
 * Sets up the watchdog timer to trigger after the defined interval if not refreshed.
 * Ensures that the interval is valid and handles errors during initialization.
 * Inputs: None (uses the global `wdtInterval` variable).
 * Outputs: None
 * Side effects: Halts the program if the watchdog timer cannot be initialized or the interval is invalid.
 */
void setupWDT() {
  if(wdtInterval < 1) {
    // Serial.println("Invalid watchdog interval");
    while(1){}
  }
  if (WDT.begin(wdtInterval)) {
    // Serial.print("WDT interval: ");
    WDT.refresh();
    // Serial.print(WDT.getTimeout());
    WDT.refresh();
    // Serial.println(" ms");
    WDT.refresh();
  } else {
    // Serial.println("Error initializing watchdog");
    while(1){}
  }
}

/*
 * Refreshes the watchdog timer to prevent a system reset.
 * Resets the watchdog timer countdown to avoid timeout. Updates the internal
 * variable `wdtMil` to keep track of the last refresh time.
 * Inputs: None
 * Outputs: None
 * Side effects: Prevents the watchdog timer from expiring, which would reset the system.
 */
void petWatchdog() {
  // if (millis() - wdtMil >= wdtInterval - 1) {
  WDT.refresh(); 
  wdtMil = millis();
  // }
}