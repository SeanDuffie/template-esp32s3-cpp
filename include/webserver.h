#pragma once

#include <LittleFS.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

extern AsyncWebServer httpServer;

void setup_webserver();
