#include "WiFiS3.h"    
#include <ArduinoHttpClient.h>
#include <UrlEncode.h>
#include "Client.h"

const char* ssid = "Brown-Guest";
const char* password = "";
int status = WL_IDLE_STATUS;

// char serverAddress[] = "192.168.86.1";  // server address
// IPAddress server(74,125,115,105);
char server[] = "api.callmebot.com";
int port = 80;
WiFiClient wifi;

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "+14018372684";
String apiKey = "4342046";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message); 
  Serial.println(url);

  HttpClient http = HttpClient(wifi, server, port);

  http.beginRequest();

  // Specify content-type header
  http.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int res = http.post(url);
  if (res == 0){
    Serial.println("Message sent successfully");
    Serial.println("Response: ");
    Serial.println(http.responseBody());
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(http.responseStatusCode());
  }

  // Free resources
  http.endRequest();
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
    status = WiFi.begin(ssid);

    // wait 1 seconds for connection:
    delay(1000);
  }

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Send Message to WhatsAPP
  // sendMessage("Hello from ESP32!");

  if (wifi.connect(server, port)) {
      Serial.println("connected");
      // Make a HTTP request:
      String message = "Hello from ESP32!";
      String url = "/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message); 
      Serial.println(url);
      wifi.print("POST " + url + " HTTP/1.1\r\n");
      wifi.print("Host: " + String(server) + "\r\n");
      wifi.print("Connection: close\r\n");
      wifi.print("Content-Type: application/x-www-form-urlencoded\r\n");
      wifi.println();

      while (wifi.connected()) {            // loop while the client's connected
        if (wifi.available()) {             // if there's bytes to read from the client,
          char c = wifi.read();             // read a byte, then
          Serial.print(c);                    // print it out to the serial monitor
        }
      }
    }
}

void loop() {}