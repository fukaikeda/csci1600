#ifdef TESTING

#include "notifications.h"

const char* ssid = "iPhone";
const char* password = "testPass";
const char* phoneNumber = "+10000000000"; // add phone number
void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    Serial.println("Running tests...");

    // Test: sendNotification with wifi not set up
    Notifications n;
    n.initNotifications();
    n.ssid = ssid;
    n.password = password;

    if (n.sendNotification(phoneNumber, n.apiKey, "Should fail") != -1) {
      Serial.println("Error: Should have failed sending notification");
    }

    n.setupWiFi();

    // Test: Basic Notification
    Serial.println("Test: Sending basic notification");
    if (n.sendNotification(phoneNumber, n.apiKey, "Test message from notifications API") == -1) {
        Serial.println("Error: Failed to send basic notification");
    }
    delay(5000);

    // Test: Encouraging Message with Name
    Serial.println("Test: Sending encouraging message with name");
    if (n.sendEncouragingMessage(phoneNumber, n.apiKey, "Jason") == -1) {
        Serial.println("Error: Failed to send encouraging message");
    }
    delay(5000);

    // Test: Long Message
    Serial.println("Test: Sending a long message");
    if (n.sendNotification(phoneNumber, n.apiKey, "This is a very long test message to ensure that the API can handle longer strings without issues.") == -1) {
        Serial.println("Error: Failed to send long message");
    }
    delay(5000);

    // Test: Special Characters in Message
    Serial.println("Test: Sending a message with special characters");
    if (n.sendNotification(phoneNumber, n.apiKey, "Hello! How are you doing today? :) #TestingAPI") == -1) {
        Serial.println("Error: Failed to send message with special characters");
    }
    delay(5000);

    // Test 5: Invalid API Key
    Serial.println("Test: Sending a message with invalid API key");
    if (n.sendNotification(phoneNumber, "invalid_key", "This should fail.") == -1) {
        Serial.println("Error: Failed to handle invalid API key");
    }
    delay(5000);

    // Test: Empty Message
    Serial.println("Test: Sending an empty message");
    if (n.sendNotification(phoneNumber, n.apiKey, "") == -1) {
        Serial.println("Error: Failed to send empty message");
    }
    delay(5000);

    // Test: Null Name in Encouraging Message
    Serial.println("Test 7: Sending encouraging message with null name");
    if (n.sendEncouragingMessage(phoneNumber, n.apiKey, "") == -1) {
        Serial.println("Error: Failed to send encouraging message with null name");
    }
    delay(5000);

    // Test: Multiple Consecutive Messages
    Serial.println("Test: Sending multiple consecutive messages");
    for (int i = 0; i < 5; i++) {
        if (n.sendNotification(phoneNumber, n.apiKey, "Consecutive message test #" + (i + 1)) == -1) {
            Serial.println("Error: Failed to send consecutive message #" + String(i + 1));
        }
        delay(1000);
    }
}

void loop() {
    // sendNotification(phoneNumber, apiKey, "Test message from notifications API");
    // sendEncouragingMessage(phoneNumber, apiKey, "Kana");
    // delay(60000);
}

#endif
