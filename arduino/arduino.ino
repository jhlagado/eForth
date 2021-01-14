#include <Arduino.h>

#include "SPIFFS.h"
#include <TFT_eSPI.h>

#include "config.h"
#include "credentials.h"
#include "dictionary.h"
#include "display.h"
#include "globals.h"
#include "http.h"
#include "network.h"
#include "util.h"
#include "vm.h"

void printNetworkInfo()
{
    Serial.println();
    Serial.println();
    Serial.print("Connected to: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Navigate to http://");
    Serial.print(deviceName);
    Serial.println(".local");
}

void showNetworkInfo(TFT_eSPI tft)
{
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.setCursor(0, 30);
    tft.setFreeFont(&Orbitron_Light_24);
    tft.println("IP");
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println(WiFi.localIP());
    tft.setCursor(0, 90);
    tft.setFreeFont(&FreeSerif12pt7b);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("http://");
    tft.print(deviceName);
    tft.println(".local");
    delay(5000);
}

static void returnFail(String msg)
{
    server.send(500, "text/plain", msg + "\r\n");
}

static void handleInput()
{
    if (!server.hasArg("cmd")) {
        return returnFail("Missing Input");
    }
    HTTPin = server.arg("cmd");
    HTTPout = "";
    Serial.println(HTTPin); // line cleaned up
    len = HTTPin.length();
    HTTPin.getBytes(cData, len);
    data[0x66] = 0; // >IN
    data[0x67] = len; // #TIB
    data[0x68] = 0; // 'TIB
    P = 0x180; // EVAL
    WP = 0x184;
    evaluate();
    server.setContentLength(HTTPout.length());
    server.send(200, "text/plain", HTTPout);
}

void setup()
{
    P = 0x180;
    WP = 0x184;
    IP = 0;
    S = 0;
    R = 0;
    top = 0;
    cData = (uint8_t*)data;

    setupNetwork(monitorSpeed, ssid, pass, deviceName);

    initDictionary();

    setupHttp(handleInput);
    printNetworkInfo();

    setupDisplay();
    showNetworkInfo(tft);
}

void loop()
{
    server.handleClient();
}
