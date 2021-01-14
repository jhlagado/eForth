#include <Arduino.h>

#include "SPIFFS.h"
#include <ESPmDNS.h>
#include <TFT_eSPI.h>
#include <WebServer.h>
#include <WiFi.h>

#include "config.h"
#include "credentials.h"
#include "dictionary.h"
#include "globals.h"
#include "index_html.h"
#include "primitives.h"
#include "vm.h"

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

TFT_eSPI tft = TFT_eSPI();
WebServer server(80);

unsigned char bytecode;

int counter = 0;
void evaluate()
{
    while (true) {
        if (counter++ > 10000) {
            delay(1);
            counter = 0;
        }
        bytecode = (unsigned char)cData[P++];
        if (bytecode) {
            primitives[bytecode]();
        } else {
            break;
        }
    } // break on NOP
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
    // Serial.print(HTTPout.length());
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

    server.begin();

    initDictionary();

    // compile \data\load.txt
    if (!SPIFFS.begin(true)) {
        Serial.println("Error mounting SPIFFS");
    }
    File file = SPIFFS.open("/load.txt");
    if (file) {
        Serial.print("\n\nLoad file: ");
        len = file.read(cData + 0x8000, 0x7000);
        Serial.print(len);
        Serial.println(" bytes.");
        data[0x66] = 0; // >IN
        data[0x67] = len; // #TIB
        data[0x68] = 0x8000; // 'TIB
        P = 0x180; // EVAL
        WP = 0x184;
        evaluate();
        Serial.println(" Done loading.");
        file.close();
        SPIFFS.end();
    }
    // Setup web server handlers
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", index_html);
    });
    server.on("/input", HTTP_POST, handleInput);
    server.begin();

    printNetworkInfo();

    tft.begin();
    tft.setRotation(1);
    showNetworkInfo(tft);
}

void loop()
{
    server.handleClient();
}
