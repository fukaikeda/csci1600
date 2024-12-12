#include "notifications.h"

/*
* Initailize a notification struct
*/
void Notifications::initNotifications() {
  ssid = "iPhone";
  password = "testPass";
  phoneNumber = "+14018372684";
  apiKey = "4342046";
  messages[0] = "Keep going, %s! You're doing great!";
  messages[1] = "%s, remember to take it one step at a time!";
  messages[2] = "You're amazing, %s! Keep it up!";
  messages[3] = "Hey %s, you can do this!";
  messages[4] = "Don't give up, %s! The best is yet to come!";
}
/* 
*   Called before sending notifications, connects to wifi for
*   notifications to work. Takes in wifi ssid and password as strings.
*/
void Notifications::setupWiFi() {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

    int status = WiFi.begin(ssid, password);
    while (status != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
        status = WiFi.status();
    }

    Serial.println("\nConnected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

/* 
*   Sends a message input to the specified phone number using apiKey.
*   WiFi must be intialized, sends to non https endpoint
*/
int Notifications::sendNotification(const char* phoneNumber, const char* apiKey, const char* message) {
    const char* server = "api.callmebot.com";
    int port = 80;

    if (!wifi.connect(server, port)) {
        Serial.println("Connection to server failed");
        return -1;
    }

    String url = String("/whatsapp.php?phone=") + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);

    Serial.println("Sending POST request to: " + String(server));
    wifi.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "User-Agent: ESP32\r\n" +
               "Connection: close\r\n\r\n");

    String response = "";
    while (wifi.connected()) {
        while (wifi.available()) {
            char c = wifi.read();
            response += c;
        }
    }

    Serial.println("Response:");
    Serial.println(response);
    return 0;
}

/* 
*   Chooses a random message from messages array, makes the message
*   personal to the name passed in and calls sendNotification to
*   send the message via WhatsApp api
*/
bool Notifications::sendEncouragingMessage(const char* name) {
  if (name == nullptr or name == "") return false;
    int numMessages = sizeof(messages) / sizeof(messages[0]);
    int randomIndex = random(0, numMessages);

    char formattedMessage[256];
    snprintf(formattedMessage, sizeof(formattedMessage), messages[randomIndex], name);

    if (sendNotification(phoneNumber, apiKey, formattedMessage) < 0) {
      Serial.println("Failed to send enocurage message notification");
      return false;
    }
    return true;
}
