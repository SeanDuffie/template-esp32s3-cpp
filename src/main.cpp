#include <Arduino.h>
#include "network.h"
#include "debug.h"
#include "ota.h"
#include "webserver.h"

static bool led_state = false;
#define RGB_LED_PIN 38

void setup() {
    Serial.begin(115200);
    // Wait for native USB CDC connection
    while (!Serial && millis() < 5000) { delay(10); }

    // 1. Establish Wi-Fi (Blocking)
    setup_wifi_manager();
    // 2. Establish Debugging ()
    setup_debug();
    // 3. Initializes LittleFS and endpoints
    setup_webserver();
    // 4. Initialize OTA Server
    setup_ota();
}


void loop() {
    // Process incoming OTA requests
    ArduinoOTA.handle();

    // Blink LED and send heartbeat every 1000ms without blocking OTA
    static uint32_t last_millis = 0;
    if (millis() - last_millis >= 1000) {
        last_millis = millis();
        led_state = !led_state;
        neopixelWrite(RGB_LED_PIN, 0, 0, led_state ? 50 : 0);
        
        debug_println("Heartbeat: System Nominal");
        handle_telnet();
    }

    if (update_requested) {
        update_requested = false; // Reset flag immediately
        debug_println("Manual OTA update triggered by user.");
        perform_ota_update(); // Your HTTPUpdate logic
    }
}
