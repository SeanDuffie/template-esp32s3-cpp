/**
 * Handles the captive portal, Wi-Fi connections, and physical resets.
 */
#include "network.h"
#include "webserver.h"
#include "debug.h"

// Define a temporary DNS server for the captive portal
DNSServer dns;

void setup_wifi_manager() {
    Serial.println("Starting Wi-Fi Manager...");

    // Local intialization. Once its business is done, there is no need to keep it around
    // Pass the async web server and dns server to the manager
    AsyncWiFiManager wm(&httpServer, &dns);

    // Reset settings for testing (Uncomment to wipe saved credentials on boot)
    // wm.resetSettings();

    // Set a custom hostname for the ESP32 on your network
    WiFi.setHostname("ESP32-S3-Template-cpp");

    // Check if the BOOT button is held down while the device turns on
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
    delay(100); // Brief debounce
    
    if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
        Serial.println("Boot button held. Wiping Wi-Fi credentials...");
        wm.resetSettings();
        
        // Blink LED rapidly to indicate wipe is complete
        for(int i = 0; i < 10; i++) {
            neopixelWrite(RGB_LED_PIN, 50, 0, 0); // Red
            delay(100);
            neopixelWrite(RGB_LED_PIN, 0, 0, 0);
            delay(100);
        }
    }

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name
    // and goes into a blocking loop awaiting configuration
    if (!wm.autoConnect("ESP32-Setup-Portal")) {
        Serial.println("Failed to connect and hit timeout");
        ESP.restart(); // Reset and try again
    } 
    
    // If you get here you have connected to the WiFi
    Serial.println("\nConnected to Wi-Fi.");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// void setup_wifi() {
//     AsyncWiFiManager wm;
//     wm.setHostname("ESP32-S3-Sensor");
//     if (!wm.autoConnect("ESP32-Setup-Portal")) {
//         ESP.restart();
//     }
// }
