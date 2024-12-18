#include "notifications.h"

/*
 * Initializes a notification struct with default values.
 * 
 * Sets up default Wi-Fi credentials, phone number, API key, and a list of predefined encouraging messages.
 * 
 * Inputs: None
 * Outputs: None
 */
void Notifications::initNotifications() {
  ssid = "Miaaa";
  password = "12345678Mia";
  phoneNumber = "+14018372684";
  apiKey = "4342046";
  message_finished = false;
  messages[0] = "Keep going, %s! You're doing great!";
  messages[1] = "%s, remember to take it one step at a time!";
  messages[2] = "You're amazing, %s! Keep it up!";
  messages[3] = "Hey %s, you can do this!";
  messages[4] = "Don't give up, %s! The best is yet to come!";
}

/*
 * Sets up Wi-Fi for message sending.
 * Called before sending notifications, connects to wifi for
 * notifications to work. Takes in wifi ssid and password as strings.
 * 
 * Inputs: None 
 * Outputs: None
 * Side effects: Blocks execution until a successful Wi-Fi connection is established and prints connection status to the Serial Monitor.
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
 * Sends a message input to the specified phone number using apiKey.
 * WiFi must be intialized, sends to non https endpoint
 * Establishes a connection to the API server, formats a URL with the phone number, API key, and message,
 * then sends a POST request.
 * 
 * Inputs:
 * phoneNumber The recipient's phone number for the message.
 * apiKey The API key for authenticating with the notification server.
 * message The message content to be sent.
 * Outputs:
 * int Returns 0 on success, or -1 if the connection or request fails.
 * 
 * Side effects: Connects to a server over Wi-Fi, sends data over the network, and prints the server response to the Serial Monitor.
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
 * Sends a personalized encouraging message via WhatsApp API.
 * 
 * Selects a random message from the predefined `messages` array, formats it with the recipient's name,
 * and sends it using `sendNotification`.
 * 
 * Inputs:
 * name The recipient's name to personalize the message.
 * Output: int Returns 0 on success, or -1 if the message fails to send.
 * 
 * Side effects: Sends a message via the network and updates the `message_finished` flag to indicate success or failure.
 */
int Notifications::sendEncouragingMessage(const char* name) {
  if (name == nullptr or name == "") return -1;
    int numMessages = sizeof(messages) / sizeof(messages[0]);
    int randomIndex = random(0, numMessages);

    char formattedMessage[256];
    snprintf(formattedMessage, sizeof(formattedMessage), messages[randomIndex], name);

    if (sendNotification(phoneNumber, apiKey, formattedMessage) < 0) {
      Serial.println("Failed to send enocurage message notification");
      message_finished = false;
      return -1;
    }

    message_finished = true;
    return 0;
}