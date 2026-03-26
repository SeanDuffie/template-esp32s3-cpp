/**
 * Handles all serial, telnet, and websocket printing.
 */
#include "debug.h"

// Define variables
AsyncWebSocket ws("/ws");
WiFiServer telnetServer(23);
WiFiClient telnetClient;
bool update_requested = false;

void setup_debug() {
    // 1. Start Telnet
    telnetServer.begin();
    telnetServer.setNoDelay(true);

    // 2. Start WebSocket Events
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        if (type == WS_EVT_CONNECT) {
            Serial.printf("WS Client connected: %u\n", client->id());
        } else if (type == WS_EVT_DISCONNECT) {
            Serial.printf("WS Client disconnected: %u\n", client->id());
        }
    });
}

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
