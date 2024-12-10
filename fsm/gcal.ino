#include "gcal.h"

void GCal::initGCal() {
  ssid = "iPhone";
  password = "testPass";
  urlPath = "https://script.google.com/macros/s/AKfycbzlxd6S-ICKerltaRHMZuLLY5uu0sigxrSwYrpZRVTKGtt7K9BRi6qSf9NOxINSZJr2yw/exec";
  homeTimes = {
    {"Mia", ""},
    {"Fuka", ""},
    {"Jason", ""},
    {"Kana", ""}
  };
}

void GCal::connectWiFi() {
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

/* 
 * Fetch data from Google API
*/
void GCal::fetchData() {
    const int maxRetries = 5;
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

            String redirectLocation = "";
            bool isRedirect = false;

            // Read and handle the response
            Serial.println("Response:");
            while (sslClient.connected() || sslClient.available()) {
                if (sslClient.available()) {
                    String line = sslClient.readStringUntil('\n');
                    Serial.println(line);

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
                delay(2000);
            } else {
                Serial.println("Max retries reached. Skipping this attempt.");
            }
        }
    }
}

/* 
 * Follow redirect URL
*/
void GCal::followRedirect(const String& redirectURL) {
    const int maxRetries = 5;
    int retryCount = 0;

    while (retryCount < maxRetries) {
        Serial.println("\nConnecting to redirected URL...");

        int index = redirectURL.indexOf("/", 8); // Skip "https://"
        String host = redirectURL.substring(8, index);
        String path = redirectURL.substring(index);

        Serial.println("Host: " + host);
        Serial.println("Path: " + path);

        if (sslClient.connect(host.c_str(), 443)) {
            Serial.println("Connected to redirected server");

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
                    line.trim();

                    for (auto& entry : homeTimes) {
                        if (line.indexOf(entry.first) != -1) {
                            String timeLine = sslClient.readStringUntil('\n');
                            timeLine.trim();

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
            return;
        } else {
            Serial.println("Connection to redirected URL failed!");
            retryCount++;
            if (retryCount < maxRetries) {
                Serial.println("Retrying...");
                delay(2000);
            } else {
                Serial.println("Max retries reached. Skipping this attempt.");
            }
        }
    }
}

void GCal::printHomeTimes() {
    Serial.println("\nHome Times:");
    for (const auto& entry : homeTimes) {
        Serial.println(entry.first + ": " + entry.second);
    }
}

/* 
*   Get the return home time from the map
*/
String GCal::getHomeTime(String name) {
  String time = homeTimes[name];
  return extractTime(time);
}


String GCal::extractTime(String timeString) {
  // Find the start and end positions of the time portion
  int timeStart = timeString.indexOf(" ") + 1; // Find first space
  timeStart = timeString.indexOf(" ", timeStart) + 1; // Skip day and month
  timeStart = timeString.indexOf(" ", timeStart) + 1; // Skip date
  timeStart = timeString.indexOf(" ", timeStart) + 1; //skip year
  int timeEnd = timeString.indexOf(" ", timeStart);
  
  // Extract the time portion (e.g., "20:00:00")
  String timePortion = timeString.substring(timeStart, timeEnd);
  Serial.println("timePortion");
  Serial.println(timePortion);
  
  // Split the time into hours and minutes
  int colonIndex = timePortion.indexOf(":");
  String hours = timePortion.substring(0, colonIndex);
  String minutes = timePortion.substring(colonIndex + 1, colonIndex + 3);
  
  // Combine hours and minutes into a 4-digit format
  String formattedTime = hours + minutes;
  return formattedTime;
}