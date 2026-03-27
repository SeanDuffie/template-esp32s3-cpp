#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>

struct TempData {
    float temperatureF; // °F
    bool valid;
};

class DS18B20Sensor {
private:
    DallasTemperature _sensors;
    uint8_t _index; // Used to identify multiple sensors on the same wire

public:
    // Pass the OneWire bus by reference, and specify which sensor on the line this is
    DS18B20Sensor(OneWire& oneWireBus, uint8_t sensorIndex = 0);

    bool begin();
    TempData readData();
};