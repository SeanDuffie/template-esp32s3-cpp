#pragma once
#include <HardwareSerial.h>
#include <TinyGPS++.h>

struct GPSData {
    double latitude;
    double longitude;
    float altitudeMeters;
    float speedMPH;
    uint32_t satellites;
    bool valid; // True only when the module has a hard satellite fix
};

class NEO6MSensor {
private:
    HardwareSerial& _serial;
    TinyGPSPlus _gps;

public:
    NEO6MSensor(HardwareSerial& serialBus);
    
    void begin();
    
    // This MUST be called continuously in your main loop
    void update(); 
    
    GPSData readData();
};