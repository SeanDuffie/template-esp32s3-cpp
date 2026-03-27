#include "neo6m_gps.h"
#include "debug.h"

NEO6MSensor::NEO6MSensor(HardwareSerial& serialBus) 
    : _serial(serialBus) {}

void NEO6MSensor::begin() {
    debug_println("NEO-6M GPS driver initialized. Awaiting satellite fix...");
}

void NEO6MSensor::update() {
    // Drain the hardware serial buffer and feed the parser.
    while (_serial.available() > 0) {
        _gps.encode(_serial.read());
    }
}

GPSData NEO6MSensor::readData() {
    GPSData data;

    // isValid() is only true when the GPS has locked onto enough satellites
    data.valid = _gps.location.isValid();
    
    // You can sometimes see satellites before you have a valid location fix
    data.satellites = _gps.satellites.isValid() ? _gps.satellites.value() : 0;

    if (data.valid) {
        data.latitude = _gps.location.lat();
        data.longitude = _gps.location.lng();
        data.altitudeMeters = _gps.altitude.isValid() ? _gps.altitude.meters() : 0.0;
        data.speedMPH = _gps.speed.isValid() ? _gps.speed.mph() : 0.0;
    } else {
        data.latitude = 0.0;
        data.longitude = 0.0;
        data.altitudeMeters = 0.0;
        data.speedMPH = 0.0;
    }

    return data;
}