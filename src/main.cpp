#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

#define RGB_LED_PIN 38 // Default WS2812 pin for ESP32-S3 DevKitC

static bool led_state = false;

// Define a temporary DNS server for the captive portal
DNSServer dns;

// Initialize Servers
WiFiServer telnetServer(23);
WiFiClient telnetClient;

AsyncWebServer httpServer(80);
AsyncWebSocket ws("/ws");

// Unified Print Function
void debug_print(const char* message) {
    // 1. Hardware Serial
    Serial.print(message);

    // 2. Telnet Client
    if (telnetClient && telnetClient.connected()) {
        telnetClient.print(message);
        telnetClient.print("\r");
    }

    // 3. WebSocket Clients
    ws.textAll(message);
}

void debug_println(const char* message) {
    debug_print(message);
    debug_print("\n");
}

void debug_printf(const char* format, ...) {
    char buffer[256];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    debug_print(buffer);
}

void setup_wifi_manager() {
    Serial.println("Starting Wi-Fi Manager...");

    // Local intialization. Once its business is done, there is no need to keep it around
    // Pass the async web server and dns server to the manager
    AsyncWiFiManager wm(&httpServer, &dns);

    // Reset settings for testing (Uncomment to wipe saved credentials on boot)
    // wm.resetSettings();

    // Set a custom hostname for the ESP32 on your network
    WiFi.setHostname("ESP32-S3-Sensor-cpp");

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

void setup_ota() {
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("esp32-s3-template");

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

void setup_debug_servers() {
    // Start Telnet
    telnetServer.begin();
    telnetServer.setNoDelay(true);

    // Start WebSockets
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            Serial.printf("WS Client connected: %u\n", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            Serial.printf("WS Client disconnected: %u\n", client->id());
        }
    });
    httpServer.addHandler(&ws);
    httpServer.begin();
}

void handle_telnet() {
    if (telnetServer.hasClient()) {
        if (!telnetClient || !telnetClient.connected()) {
            if (telnetClient) telnetClient.stop();
            telnetClient = telnetServer.available();
            Serial.println("New Telnet client connected.");
        } else {
            WiFiClient temp = telnetServer.available();
            temp.stop();
        }
    }

    // FIX: Clear incoming negotiation bytes so the socket doesn't lock up
    if (telnetClient && telnetClient.connected() && telnetClient.available()) {
        while (telnetClient.available()) {
            telnetClient.read(); 
        }
    }
}

void setup() {
    Serial.begin(115200);
    // Wait for native USB CDC connection
    while (!Serial && millis() < 5000) { delay(10); }

    // 1. Establish Wi-Fi (Blocking)
    setup_wifi_manager();
    Serial.printf("\nConnected. IP: %s\n", WiFi.localIP().toString().c_str());

    // 2. Establish Debugging ()
    setup_debug_servers();
    debug_printf("\nConnected Debuggers. IP: %s\n", WiFi.localIP().toString().c_str());

    // 3. Initialize OTA Server
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

        if (led_state) {
            neopixelWrite(RGB_LED_PIN, 0, 0, 50); // Blue
        } else {
            neopixelWrite(RGB_LED_PIN, 0, 0, 0);  // Off
        }

        debug_println("Heartbeat: System Nominal");

        handle_telnet();
    }
}