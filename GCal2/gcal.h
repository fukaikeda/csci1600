#ifndef GCAL_H
#define GCAL_H

#include <WiFiS3.h>
#include <map>
#include <Arduino.h>

struct GCal {
    // const char* ssid;
    // const char* password;
    const char* urlPath;
    WiFiSSLClient sslClient;
    std::map<String, String> homeTimes;

    // void connectWiFi();
    void initGCal();
    void fetchData();
    void printHomeTimes();
    void followRedirect(const String& redirectURL);
    String getHomeTime(String name);
};

#endif // GCAL_H
