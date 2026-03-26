/**
 * Handles both the push (ArduinoOTA) and pull (HTTPUpdate) firmware routines.
 */
#include "ota.h"
#include "debug.h"

void setup_ota() {
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    ArduinoOTA.setHostname("ESP32-S3-Template-cpp");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_SPIFFS
            type = "filesystem";
        }
        // NOTE: if updating SPIFFS/LittleFS this would be the place to unmount it
        debug_printf("Start updating %s", type);
    });
    
    ArduinoOTA.onEnd([]() {
        debug_println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        debug_printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        debug_printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) debug_println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) debug_println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) debug_println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) debug_println("Receive Failed");
        else if (error == OTA_END_ERROR) debug_println("End Failed");
    });

    ArduinoOTA.begin();
    debug_println("OTA Ready.");
}

void perform_ota_update() {
    WiFiClient client;
    // Point this to your local server hosting the .bin file
    t_httpUpdate_return ret = httpUpdate.update(client, "http://192.168.1.100/firmware.bin");

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;
      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
}