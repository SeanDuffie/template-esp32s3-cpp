#pragma once
#include <Wire.h>
#include <BH1750.h>

struct LuxData {
    float lux;
    bool valid;
};

class BH1750Sensor {
private:
    BH1750 _lightMeter;
    TwoWire& _wire;
    uint8_t _address;

public:
    // The GY-302 defaults to 0x23. If the ADDR pin is pulled high, it becomes 0x5C.
    BH1750Sensor(TwoWire& wireBus, uint8_t address = 0x23);
    
    bool begin();
    LuxData readData();
};