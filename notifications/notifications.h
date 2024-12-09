// notifications.h
#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <Arduino.h>
#include "WiFiS3.h"
#include <ArduinoHttpClient.h>
#include <UrlEncode.h>

struct Notifications {
  const char* ssid;
  const char* password;
  const char* phoneNumber;
  const char* apiKey;
  WiFiClient wifi;
  const char* messages[5];
  void initNotifications();
  void setupWiFi();
  int sendNotification(const char* phoneNumber, const char* apiKey, const char* message);
  int sendEncouragingMessage(const char* phoneNumber, const char* apiKey, const char* name);
};

#endif // NOTIFICATIONS_H