#pragma once

#include <Arduino.h>
#include <WebServer.h>

extern WebServer server;
extern String HTTPin;
extern String HTTPout;

void setupHttp(void (*handleInput)());
