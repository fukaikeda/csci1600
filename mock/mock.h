struct mockNotifications {
  void setupWiFi(const char* ssid, const char* password);
  int sendNotification(const char* phoneNumber, const char* apiKey, const char* message) { return 0; }
  int sendEncouragingMessage(const char* phoneNumber, const char* apiKey, const char* name) { return 0; }
};

struct mockCalendar {
    void connectWiFi() { Serial.println("Connected to WiFi"); }
    void fetchData() { Serial.println("Fetched Data"); };
    // void printHomeTimes();
    // void followRedirect(const String& redirectURL);
    String getHomeTime(String name) { return "Jason"; };
};

struct mockClock {
  void updateClock(int hour, int minute) { Serial.println("Clock updated"); }
};
