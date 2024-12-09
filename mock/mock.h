struct mockNotifications {
  void setupWiFi(const char* ssid, const char* password);
  int sendNotification(const char* phoneNumber, const char* apiKey, const char* message) { return 0; }
  int sendEncouragingMessage(const char* phoneNumber, const char* apiKey, const char* name) { return 0; }
};

struct mockCalendar {

};

struct mockClock {
  void updateClock(int hour, int minute) { Serial.println("Clock updated"); }
};
