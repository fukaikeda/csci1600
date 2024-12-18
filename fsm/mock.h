#ifndef MOCK_H
#define MOCK_H

#include <Arduino.h>

/**
 * Mock implementation of notifications for testing.
 * Simulates sending notifications and messages without actual communication.
 */
struct mockNotifications {
  bool message_finished = false;
  /*
   * Sets up mock Wi-Fi with provided credentials.
   * Inputs:
   * ssid The SSID of the Wi-Fi network.
   * password The password for the Wi-Fi network.
   * Outputs: None
   * Side effects: Prints mock Wi-Fi setup to the serial monitor.
   */
  void setupWiFi(const char* ssid, const char* password) {
    Serial.println("Mock WiFi setup with SSID: " + String(ssid));
  }

  /*
   * Sends a mock notification.
   * 
   * Inputs:
   * phoneNumber The recipient's phone number.
   * apiKey The API key for sending the notification.
   * message The content of the notification message.
   * Outputs:
   * int Returns 0 indicating success.
   * Side effects: Simulates sending a notification.
   */
  int sendNotification(const char* phoneNumber, const char* apiKey, const char* message) {
    return 0;
  }
  /*
   * Sends a mock encouraging message to a user.
   * Inputs:
   * name The name of the user to send the message to.
   * Outputs:
   * int Returns 0 indicating success.
   * Side effects: Updates the message_finished flag to true.
  */
  int sendEncouragingMessage(const char* name) {
    message_finished = true;
    return 0;
  }
};

/*
 * Mock implementation of a calendar system for testing.
 * Simulates calendar operations such as fetching data and retrieving home times.
 */
struct mockCalendar {
  /*
   * Simulates connecting to Wi-Fi for calendar operations.
   * Inputs: None
   * Outputs: None
   * Side effects: Prints a connection message to the serial monitor.
   */
  void connectWiFi() { 
    Serial.println("Mock WiFi connected for Calendar"); 
  }

  /*
   * Simulates fetching calendar data.
   * Inputs: None
   * Outputs: None
   * Side effects: Prints a fetch message to the serial monitor.
   */
  void fetchData() { 
    Serial.println("Mock calendar data fetched"); 
  }

  /**
   * Retrieves a mock home time for a specified user.
   * 
   * Input:
   * name The name of the user.
   * Outputs:
   * String Returns a mock time "18:30" for the user.
   * Side effects: Prints a retrieval message to the serial monitor.
   */
  String getHomeTime(String name) { 
    Serial.println("Mock getHomeTime called for name: " + name);
    return "18:30";
  }
};

/*
 * Mock implementation of a clock controller for testing.
 * Simulates clock operations such as updating and displaying real-time mode.
 */
struct mockClock {
  bool displayRealTime = false;

  /*
   * Updates the mock clock to a specified time.
   * 
   * Inputs:
   * hour The hour to update the clock to.
   * Outputs:
   * minute The minute to update the clock to.
   * Side effects: Prints the updated time to the serial monitor.
   */
  void updateClock(int hour, int minute) { 
    Serial.print("Mock clock updated to: ");
    Serial.print(hour);
    Serial.print(":");
    Serial.println(minute); 
  }

  /*
   * Handles input mode for the mock clock.
   * 
   * Inputs:
   * time A string representing the time to set the clock to.
   * Outputs:
   * bool Returns true indicating success.
   * Side effects: Prints the input time to the serial monitor.
   */
  bool handleInputMode(String time) {
    Serial.println("Mock clock handleInputMode called with time: " + time);
    return true;
  }

  /*
   * Simulates the clock operating in real-time mode.
   * Inputs: None
   * Outputs: None
   * Side effects: Prints a real-time mode message to the serial monitor.
   */
  void handleRealTimeMode() {
    Serial.println("Mock real-time mode called");
  }
};

#endif // MOCK_H
