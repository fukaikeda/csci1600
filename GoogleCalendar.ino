
#include <WiFiS3.h>
#include <R4HttpClient.h>

// Network credentials
const char* ssid = "";       // Replace with your Wi-Fi SSID
const char* password = ""; // Replace with your Wi-Fi password

// Others to know
// google script link: https://script.google.com/d/19unH3aEThqvhhvN5cS-7wBeEpn8J3gnp18I5Gien4n2hUJxdyQV3oPU_/edit?usp=sharing
// google calender shared link: https://calendar.google.com/calendar/u/0?cid=Y18wMzllODQyYzg3N2QzN2Q4YjI5ODgzZmYwYzc2ODlmYzczZDViMjg0MWVkNTQyMjI5ZjQyMzcwNDkwOTBkYzNiQGdyb3VwLmNhbGVuZGFyLmdvb2dsZS5jb20 

// Google API URL
const char* url = "https://script.google.com/macros/s/AKfycbz0sL1mPWxNm7s4fAQo-7UQgS-n01ZrbD-E0JqsV50/exec"; 
// do not change

WiFiSSLClient sslClient;  // SSL client for secure connection
R4HttpClient http;        // HTTP client for making requests

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Check Wi-Fi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true); // Stop here
  }

  // Check firmware version
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  fetchDataFromGoogle();
}

void loop() {
  // Repeatedly fetch data every 60 seconds
  delay(60000);
  fetchDataFromGoogle();
}

void fetchDataFromGoogle() {
  Serial.println("Connecting to Google API...");

  // Initialize HTTP connection
  http.begin(sslClient, url, 443);  // Use WiFiSSLClient for secure HTTPS connection
  http.setTimeout(5000);           // Set timeout in milliseconds

  // Add headers as a single string
  http.addHeader("User-Agent: Arduino UNO R4 WiFi");
  http.addHeader("Connection: close");

  // Make GET request
  int httpCode = http.GET();
  if (httpCode > 0) {
    // Successful request
    String responseBody = http.getBody();
    Serial.println("Response received:");
    Serial.println(responseBody);

    // Process the response
    parseAndPrintCalendarData(responseBody);
  } else {
    // Request failed
    Serial.print("HTTP GET failed with error code: ");
    Serial.println(httpCode);
  }

  // Close HTTP connection
  http.close();
}

void parseAndPrintCalendarData(const String& data) {
  Serial.println("Parsing calendar data...");

  int start = 0, end = data.indexOf('|', start);
  while (end != -1) {
    String task = data.substring(start, end);
    start = end + 1;
    end = data.indexOf('|', start);

    if (end == -1) break; // Prevent substring out of range if data format is incorrect

    String time = data.substring(start, end);
    start = end + 1;
    end = data.indexOf('|', start);

    // Print task name and time
    Serial.print("Task: ");
    Serial.print(task);
    Serial.print(", Time: ");
    Serial.println(time);
  }
}

// // orginial code to refer to

// const char* host = "script.google.com";
// const char* googleRedirHost = "script.googleusercontent.com";
// const char* hostFingerprint = "42:59:51:7C:D4:E4:8A:28:9D:33:2A:B3:F0:AB:52:A3:66:32:28:24miayu@MiasMac-852"; //replace with fingerprint for host
// // const char* googleRedirHostFingerprint = "redirectFingerprint"; // replace with the fingerprint for the redirected host
// const int httpsPort = 443;
// const String delimeter = "|";

// void fetchDataFromGoogle() {
//   HTTPSRedirect client(httpsPort);
//   client.setFingerprint(hostFingerprint);
//   if (!client.connected())
//     client.connect(host, httpsPort);

//   String data = client.getData(url, host, googleRedirHost);

//   bool p = false;
//   for (int i = 0; i < data.length(); i++) {
//     if ((String)data[i] == delimeter)
//       p = !p;

//     if (p && (String)data[i] != delimeter) {
//       if (isdigit(data[i])) {
//         activePins[data[i] - '0'] = true;
//       }
//     }
//   }
// }

