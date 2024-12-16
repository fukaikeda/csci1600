#include "mock.h"

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mockNotifications mock;

}

void loop() {
}