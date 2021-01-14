#include <Arduino.h>

#include <ESPmDNS.h>
#include <WiFi.h>

void setupNetwork(int monitorSpeed, const char* ssid, const char* pass, const char* deviceName)
{
    Serial.begin(monitorSpeed);
    delay(100);

    // attempt to connect to Wifi network:
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    if (!MDNS.begin(deviceName)) {
        Serial.print("Error starting MDNS");
    }
}
