
#include "WiFiS3.h"

// Wi-Fi credentials
const char* ssid = "Miaaa";       // Replace with your Wi-Fi SSID
const char* password = "12345678Mia"; // Replace with your Wi-Fi password

// Public Google Apps Script URL
const char* url = "https://script.google.com/macros/s/AKfycbxpnlO-_BfrIMJvIon-8f0RxdPNq7DXnHmivwoqN6PJSGaR2ZtujgbDSgtjJH6Zy85X/exec";

// SSL client
WiFiSSLClient sslClient;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Fetch initial data
  fetchDataFromGoogle();
}

void loop() {
  delay(60000); // Wait for 60 seconds
  fetchDataFromGoogle();
}

void fetchDataFromGoogle() {
  Serial.println("\nConnecting to Google API...");

  if (sslClient.connect("script.google.com", 443)) {
    Serial.println("Connected to server");

    // Send HTTP GET request
    sslClient.println("GET /macros/s/AKfycbxpnlO-_BfrIMJvIon-8f0RxdPNq7DXnHmivwoqN6PJSGaR2ZtujgbDSgtjJH6Zy85X/exec HTTP/1.1");
    sslClient.println("Host: script.google.com");
    sslClient.println("Connection: close");
    sslClient.println();

    // Read and print the response
    Serial.println("Response:");
    while (sslClient.connected() || sslClient.available()) {
      if (sslClient.available()) {
        String line = sslClient.readStringUntil('\n');
        Serial.println(line); // Print each line of the response
      }
    }

    sslClient.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection failed!");
  }
}


// void fetchDataFromGoogle() {
//   Serial.println("\nConnecting to Google API...");

//   // Connect to the host
//   if (sslClient.connect("script.google.com", 443)) {
//     Serial.println("Connected to server");

//     // Send HTTP GET request
//     sslClient.println("GET /macros/library/d/19unH3aEThqvhhvN5cS-7wBeEpn8J3gnp18I5Gien4n2hUJxdyQV3oPU_/1 HTTP/1.1");
//     sslClient.println("Host: script.google.com");
//     sslClient.println("Connection: close");
//     sslClient.println();

//     String responseBody = "";
//     while (sslClient.connected() || sslClient.available()) {
//       if (sslClient.available()) {
//         String line = sslClient.readStringUntil('\n');
//         responseBody += line; // Collect the response body
//       }
//     }

//     Serial.println("Response:");
//     Serial.println(responseBody);

//   }
// }


