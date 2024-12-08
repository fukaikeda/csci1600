#include "notifications.h"

WiFiClient wifi;
const char* messages[] = {
    "Keep going, %s! You're doing great!",
    "%s, remember to take it one step at a time!",
    "You're amazing, %s! Keep it up!",
    "Hey %s, you can do this!",
    "Don't give up, %s! The best is yet to come!"
};

/* 
*   Called before sending notifications, connects to wifi for
*   notifications to work. Takes in wifi ssid and password as strings.
*/
void setupWiFi(const char* ssid, const char* password) {
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
void sendNotification(const char* phoneNumber, const char* apiKey, const char* message) {
    const char* server = "api.callmebot.com";
    int port = 443;

    if (!wifi.connect(server, port)) {
        Serial.println("Connection to server failed");
        return;
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
}

/* 
*   Chooses a random message from messages array, makes the message
*   personal to the name passed in and calls sendNotification to
*   send the message via WhatsApp api
*/
void sendEncouragingMessage(const char* phoneNumber, const char* apiKey, const char* name) {
    int numMessages = sizeof(messages) / sizeof(messages[0]);
    int randomIndex = random(0, numMessages);

    char formattedMessage[256];
    snprintf(formattedMessage, sizeof(formattedMessage), messages[randomIndex], name);

    sendNotification(phoneNumber, apiKey, formattedMessage);
}
