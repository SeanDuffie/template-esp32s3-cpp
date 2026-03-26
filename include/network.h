#pragma once

#include <ESPAsyncWiFiManager.h>
#include <Arduino.h>
// #include <WiFi.h>
// #include <ESPAsyncWebServer.h>

#define BOOT_BUTTON_PIN 0 // BOOT button on ESP32-S3
#define RGB_LED_PIN 38 // Default WS2812 pin for ESP32-S3 DevKitC

void setup_wifi_manager();
// void setup_wifi();
