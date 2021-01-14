#include <Arduino.h>

// #include "SPIFFS.h"
// #include <ESPmDNS.h>
#include <TFT_eSPI.h>
// #include <WiFi.h>

// #include "config.h"
// #include "credentials.h"
// #include "dictionary.h"
// #include "globals.h"
// #include "vm.h"
// #include "util.h"
// #include "http.h"

TFT_eSPI tft = TFT_eSPI();

void setupDisplay()
{
    tft.begin();
    tft.setRotation(1);
}
