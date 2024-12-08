// notifications.h
#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <Arduino.h>
#include "WiFiS3.h"
#include <ArduinoHttpClient.h>
#include <UrlEncode.h>

void setupWiFi(const char* ssid, const char* password);
void sendNotification(const char* phoneNumber, const char* apiKey, const char* message);
void sendEncouragingMessage(const char* phoneNumber, const char* apiKey, const char* name);

#endif // NOTIFICATIONS_H