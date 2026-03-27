/**
 * Handles static file serving, API routes, and LittleFS.
 */
#include "webserver.h"
#include "debug.h" // Gives access to 'ws' and 'update_requested'

AsyncWebServer httpServer(80);

void setup_webserver() {
    // 1. Initialize File System
    if(!LittleFS.begin(true)){
        debug_println("An Error has occurred while mounting LittleFS");
        return;
    }

    // 2. Define API Routes
    httpServer.on("/api/update", HTTP_POST, [](AsyncWebServerRequest *request){
        update_requested = true;
        request->send(200, "application/json", "{\"status\":\"updating\"}");
    });

    // Note: You still need to implement the POST handler for /api/wifi here. 
    // This requires a JSON parsing library like AsyncJson to read the payload.

    // 3. Mount WebSockets (Defined in debug.cpp)
    httpServer.addHandler(&ws);

    // 4. Serve Frontend
    httpServer.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    // 5. Start Server
    httpServer.begin();
    debug_println("Web server started.");
}