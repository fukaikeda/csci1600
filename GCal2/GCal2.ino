
#include "WiFiS3.h"

// // Wi-Fi credentials
// const char* ssid = "Miaaa";       // Replace with your Wi-Fi SSID
// const char* password = "12345678Mia"; // Replace with your Wi-Fi password

const char* ssid = "RLAB";       // Replace with your Wi-Fi SSID
const char* password = "metropolis"; // Replace with your Wi-Fi password

// mia's peronsal account google app script url 
const char* urlPath= "https://script.google.com/macros/s/AKfycbzlxd6S-ICKerltaRHMZuLLY5uu0sigxrSwYrpZRVTKGtt7K9BRi6qSf9NOxINSZJr2yw/exec";

// SSL client
WiFiSSLClient sslClient;

void setup() {
  connectWifi();
  fetchDataFromGoogle();
}

void loop() {
  delay(60000); // Wait for 60 seconds
  fetchDataFromGoogle();
}

void connectWifi(){

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
}

void fetchDataFromGoogle() {
  Serial.println("\nConnecting to Google API...");

  if (sslClient.connect("script.google.com", 443)) {
    Serial.println("Connected to server");

    // Send HTTP GET request
    sslClient.println(String("GET ") + urlPath + " HTTP/1.1");
    sslClient.println("Host: script.google.com");
    sslClient.println("Connection: close");
    sslClient.println();

    // Read and handle the response
    Serial.println("Response:");
    String redirectLocation = "";
    bool isRedirect = false;

    while (sslClient.connected() || sslClient.available()) {
      if (sslClient.available()) {
        String line = sslClient.readStringUntil('\n');
        Serial.println(line); // Print each line of the response

        // Look for redirect
        if (line.startsWith("Location: ")) {
          redirectLocation = line.substring(10);
          redirectLocation.trim();
          isRedirect = true;
        }
      }
    }

    sslClient.stop();
    Serial.println("Disconnected from server");

    // Handle redirect
    if (isRedirect && !redirectLocation.isEmpty()) {
      Serial.println("Redirect detected to: " + redirectLocation);
      followRedirect(redirectLocation);
    }
  } else {
    Serial.println("Connection failed!");
  }
}

void followRedirect(const String& redirectURL) {
  Serial.println("\nConnecting to redirected URL...");

  // Extract the host and path from the redirect URL
  int index = redirectURL.indexOf("/", 8); // Skip "https://"
  String host = redirectURL.substring(8, index);
  String path = redirectURL.substring(index);

  Serial.println("Host: " + host);
  Serial.println("Path: " + path);

  if (sslClient.connect(host.c_str(), 443)) {
    Serial.println("Connected to redirected server");

    // Send HTTP GET request to the redirected URL
    sslClient.println(String("GET ") + path + " HTTP/1.1");
    sslClient.println("Host: " + host);
    sslClient.println("Connection: close");
    sslClient.println();

    // Read and print the response
    Serial.println("Response from redirected URL:");
    while (sslClient.connected() || sslClient.available()) {
      if (sslClient.available()) {
        String line = sslClient.readStringUntil('\n');
        Serial.println(line); // Print each line of the response
      }

    }

    sslClient.stop();
    Serial.println("Disconnected from redirected server");
  } else {
    Serial.println("Connection to redirected URL failed!");
  }
}


