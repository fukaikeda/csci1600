#ifdef RUN_TESTS

#include "WiFiS3.h"
#include <map>
#include <String>
#include "gcal.h"

// Wi-Fi Credentials
const char* ssid = "iPhone";
const char* password = "testPass";

// Google Script URL
const char* urlPath = "https://script.google.com/macros/s/AKfycbzlxd6S-ICKerltaRHMZuLLY5uu0sigxrSwYrpZRVTKGtt7K9BRi6qSf9NOxINSZJr2yw/exec";

GCal g;
WiFiSSLClient sslClient;

// Global map to store return times
std::map<String, String> homeTimes = {
    {"Mia", ""},
    {"Fuka", ""},
    {"Jason", ""},
    {"Kana", ""}
};

/*
 * Sets up the test environment.
 * 
 * Initializes serial communication, runs GCal tests for various scenarios,
 * including Wi-Fi connectivity, redirect handling, and data parsing.
 * Inputs: None
 * Outputs; None
 */
void setup() {
    Serial.begin(9600);
    while (!Serial) {}

    Serial.println("Starting GCal Tests...");

    g.initGCal();

    // Test 1: Fetch Data Without Wi-Fi
    Serial.println("\nTest 1: Fetch Data Without Wi-Fi");
    if (g.fetchData() != -1) {
        Serial.println("Error: Fetch data should fail without Wi-Fi");
    }

    // Setup Wi-Fi
    Serial.println("\nSetting up Wi-Fi...");
    connectWifi();

    // Test 2: Fetch Data with Wi-Fi
    Serial.println("\nTest 2: Fetch Data with Wi-Fi");
    if (g.fetchData() == -1) {
        Serial.println("Error: Failed to fetch data with Wi-Fi");
    } else {
        printHomeTimes();
    }

    // Test 3: Handle Redirects
    Serial.println("\nTest 3: Handling Redirects");
    String redirectURL = "https://redirect.example.com/path";
    followRedirect(redirectURL);

    // Test 4: Data Parsing and Storage
    Serial.println("\nTest 4: Parsing and Storing Data");
    g.fetchData();
    printHomeTimes();

    Serial.println("\nGCal Tests Completed.");
}

/*
 * Main loop for periodic testing.
 * Periodically fetches data every 20 seconds and prints the updated home times.
 * Inputs: None
 * Outputs; None
 */
void loop() {
    // Test Fetch Data Periodically
    delay(20000); // Wait for 20 seconds
    Serial.println("\nFetching Data Periodically...");
    g.fetchData();
    printHomeTimes();
}

/*
 * Connects to the specified Wi-Fi network.
 * Attempts to connect to the Wi-Fi network using the provided credentials. Prints
 * connection status and IP address upon successful connection.
 * Inputs: None
 * Outputs; None
 * Side effect: Changes wifi oebect state
 */
void connectWifi() {
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
 * Prints the stored return times for users.
 * Iterates over the global homeTimes map and prints each user's name and
 * their corresponding return time.
 * Inputs: None
 * Outputs; None
 */
void printHomeTimes() {
    Serial.println("\nHome Times:");
    for (const auto& entry : homeTimes) {
        Serial.println(entry.first + ": " + entry.second);
    }
}

#endif




