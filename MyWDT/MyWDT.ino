#include "MyWDT.h"

void setup() {
  Serial.begin(9600);
  delay(1000); // Allow time for Serial monitor to open
  Serial.println("Setting up watchdog...");

  // Initialize the watchdog timer
  setupWDT();

  Serial.println("Watchdog setup complete.");
}

void loop() {
  // Simulate some work
  Serial.println("Working...");

  // Feed (pet) the watchdog to prevent a reset
  petWatchdog();

  // Simulate a delay or task that must complete
  delay(1000);

  // Uncomment the line below to simulate a hang, which will cause a watchdog reset
  // while (true) {}
}
