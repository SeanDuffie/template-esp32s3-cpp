#pragma once
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

// Initialize Servers
extern AsyncWebSocket ws;
extern WiFiServer telnetServer;
extern bool update_requested;

void setup_debug();
void debug_print(const char* message);
void debug_println(const char* message);
void debug_printf(const char* format, ...);
void handle_telnet();
