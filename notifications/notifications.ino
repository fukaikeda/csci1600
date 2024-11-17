#include "WiFiS3.h"    
#include <ArduinoHttpClient.h>
#include <UrlEncode.h>
#include "Client.h"

const char* ssid = "iPhone";
const char* password = "testPass";
int status = WL_IDLE_STATUS;

// char serverAddress[] = "192.168.86.1";  // server address
// IPAddress server(74,125,115,105);
char server[] = "api.callmebot.com";
int port = 443;
WiFiClient wifi;

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+14018372684";
String apiKey = "4342046";

void redirectCallback(String response) {
    int urlIndex = response.indexOf("url=");
    if (urlIndex == -1) {
        Serial.println("Redirect URL not found.");
        return;
    }

    // Extract the URL
    int start = urlIndex + 4; // "url="
    int end = response.indexOf("'", start);
    if (end == -1) {
        Serial.println("Invalid URL format");
        return;
    }

    String redirectUrl = response.substring(start, end);
    Serial.println("Redirect URL: " + redirectUrl);

    int pathIndex = redirectUrl.indexOf("/", 8); // http:// or https://
    String host = redirectUrl.substring(7, pathIndex);
    String path = redirectUrl.substring(pathIndex);

    Serial.println("Host: " + host);
    Serial.println("Path: " + path);

    if (!wifi.connect(host.c_str(), 80)) {
        Serial.println("Failed to connect to redirect server.");
        return;
    }

    // Send GET request
    wifi.print(String("GET ") + path + " HTTPS/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n" +
                         "\r\n");

    String redirectResponse = "";
    while (wifi.connected()) {
        while (wifi.available()) {
            char c = wifi.read();
            redirectResponse += c;
        }
    }

    Serial.println("Redirect Response:");
    Serial.println(redirectResponse);
}

void sendMessage(String message) {
    if (!wifi.connect(server, port)) {
        Serial.println("Connection to server failed");
        return;
    }

    String url = "/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);
    Serial.println("Sending POST request to: " + String(server));
    wifi.print(String("POST ") + url + " HTTPS/1.1\r\n" +
               "Host: api.callmebot.com\r\n" +
               "User-Agent: ESP32\r\n" +
               "Connection: close\r\n" +
               "\r\n");

    while (wifi.connected()) {
        String line = wifi.readStringUntil('\n');
        if (line == "\r") break;
        Serial.println(line);
    }

    String response = "";
    while (wifi.available()) {
        String line = wifi.readStringUntil('\n');
        response += line;
    }
    Serial.println("Response:");
    Serial.println(response);

    redirectCallback(response);
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // String fv = WiFi.firmwareVersion();
  // if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
  //   Serial.println("Please upgrade the firmware");
  // }

  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (status != WL_CONNECTED) {
    Serial.print(".");
    // Connect to WPA/WPA2 network:
    // status = WiFi.begin(ssid);
    status = WiFi.begin(ssid, password);

    // wait 1 seconds for connection:
    delay(1000);
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Check if a captive portal exists, for Brown-Guest
  if (isCaptivePortal()) {
      Serial.println("Captive portal detected!");
  } else {
      Serial.println("No captive portal detected.");
  }
}

bool isCaptivePortal() {
    if (!wifi.connect("connectivitycheck.gstatic.com", 80)) {
        Serial.println("Failed to connect to test server.");
        return false;
    }

    wifi.print(String("GET /generate_204 HTTP/1.1\r\n") +
               "Host: connectivitycheck.gstatic.com\r\n" +
               "Connection: close\r\n\r\n");

    String response = "";
    while (wifi.connected()) {
        while (wifi.available()) {
            char c = wifi.read();
            response += c;
        }
    }

    Serial.println("Captive Portal Check Response:");
    Serial.println(response);

    return response.indexOf("HTTP/1.1 204 No Content") == -1;
}

void loop() {
  if (status != WL_CONNECTED) return;
  sendMessage("Sent from arudino, it worked for real this time");
  delay(100000);
}