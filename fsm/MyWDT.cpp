#include "MyWDT.h"

unsigned long wdtMil = 0;
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

void petWatchdog() {
  // if (millis() - wdtMil >= wdtInterval - 1) {
  WDT.refresh(); 
  wdtMil = millis();
  // }
}