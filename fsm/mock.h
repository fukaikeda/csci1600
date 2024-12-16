#ifndef MOCK_H
#define MOCK_H

#include <Arduino.h>

struct mockNotifications {
  bool message_finished = false;
  void setupWiFi(const char* ssid, const char* password) {
    Serial.println("Mock WiFi setup with SSID: " + String(ssid));
  }

  int sendNotification(const char* phoneNumber, const char* apiKey, const char* message) {
    return 0;
  }

  int sendEncouragingMessage(const char* name) {
    return 0;
  }
};

struct mockCalendar {
  void connectWiFi() { 
    Serial.println("Mock WiFi connected for Calendar"); 
  }

  void fetchData() { 
    Serial.println("Mock calendar data fetched"); 
  }

  String getHomeTime(String name) { 
    Serial.println("Mock getHomeTime called for name: " + name);
    return "18:30";
  }
};

struct mockClock {
  bool displayRealTime = false;
  void updateClock(int hour, int minute) { 
    Serial.print("Mock clock updated to: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.println(minute); 
  }

  bool handleInputMode(String time) {
    Serial.println("Mock clock handleInputMode called with time: " + time);
    return true;
  }

  void handleRealTimeMode() {
    Serial.println("Mock real-time mode called");
  }
};

#endif // MOCK_H
