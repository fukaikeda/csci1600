
#include "WiFiS3.h"
#include <map>
#include <String>
#include "gcal.h"

// const char* ssid = "Miaaa";       // Replace with your Wi-Fi SSID
// const char* password = "12345678Mia"; // Replace with your Wi-Fi password

const char* ssid = "iPhone";       // Replace with your Wi-Fi SSID
const char* password = "testPass"; // Replace with your Wi-Fi password

// Google Script URL 
const char* urlPath= "https://script.google.com/macros/s/AKfycbzlxd6S-ICKerltaRHMZuLLY5uu0sigxrSwYrpZRVTKGtt7K9BRi6qSf9NOxINSZJr2yw/exec";
GCal g;

// SSL client
WiFiSSLClient sslClient;

std::map<String, String> homeTimes = {
    {"Mia", ""},
    {"Fuka", ""},
    {"Jason", ""},
    {"Kana", ""}
};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  g.initGCal();
  // g.connectWiFi();
  g.fetchData();
  Serial.println("\nHome Times loop:");
  for (const auto& entry : homeTimes) {
    Serial.println(entry.first + ": " + entry.second);
  }
  
}

void loop() {
  delay(20000); // Wait for 60 seconds
  g.fetchData();
  Serial.println("\nHome Times loop:");
  
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
  const int maxRetries = 5; // Maximum number of retries
  int retryCount = 0;

  while (retryCount < maxRetries) {
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

      // Handle redirect if needed
      if (isRedirect && !redirectLocation.isEmpty()) {
        Serial.println("Redirect detected to: " + redirectLocation);
        followRedirect(redirectLocation);
      }
      
      return; // Exit function on success
    } else {
      Serial.println("Connection failed!");
      retryCount++;
      if (retryCount < maxRetries) {
        Serial.println("Retrying...");
        delay(2000); // Wait for 2 seconds before retrying
      } else {
        Serial.println("Max retries reached. Skipping this attempt.");
      }
    }
  }
}

void followRedirect(const String& redirectURL) {
  const int maxRetries = 5; // Maximum number of retries
  int retryCount = 0;

  while (retryCount < maxRetries) {
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

      const char* daysOfWeek[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
      int numDays = 7;

      Serial.println("Filtering response from redirected URL:");
      while (sslClient.connected() || sslClient.available()) {
        if (sslClient.available()) {
          String line = sslClient.readStringUntil('\n');
          line.trim(); // Remove extra whitespace

          // Check if the line contains a home time for a person
          for (auto& entry : homeTimes) {
            if (line.indexOf(entry.first) != -1) {
              // Capture the subsequent line as the time if it starts with a day
              String timeLine = sslClient.readStringUntil('\n');
              timeLine.trim();

              // Check if the time line starts with a valid day
              for (int i = 0; i < numDays; i++) {
                if (timeLine.startsWith(daysOfWeek[i])) {
                  entry.second = timeLine;
                  break;
                }
              }
            }
          }
        }
      }

      sslClient.stop();
      Serial.println("Disconnected from redirected server");
      return; // Exit function on success
    } else {
      Serial.println("Connection to redirected URL failed!");
      retryCount++;
      if (retryCount < maxRetries) {
        Serial.println("Retrying...");
        delay(2000); // Wait for 2 seconds before retrying
      } else {
        Serial.println("Max retries reached. Skipping this attempt.");
      }
    }
  }
}




