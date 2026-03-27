#include "bh1750_light.h"
#include "debug.h"

BH1750Sensor::BH1750Sensor(TwoWire& wireBus, uint8_t address) 
    : _wire(wireBus), _address(address) {}

bool BH1750Sensor::begin() {
    // Initialize in high resolution continuous mode (1 lux precision, 120ms measurement time)
    if (!_lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, _address, &_wire)) {
        debug_printf("BH1750 initialization failed at address 0x%02X. Check wiring.\n", _address);
        return false;
    }

    debug_printf("BH1750 initialized successfully at 0x%02X.\n", _address);
    return true;
}

LuxData BH1750Sensor::readData() {
    LuxData data;
    
    // Read the raw lux value
    float luxLevel = _lightMeter.readLightLevel();

    // The BH1750 library returns negative values (-1.0 or -2.0) if the I2C read fails
    if (luxLevel < 0.0) {
        data.valid = false;
        data.lux = 0.0;
        debug_println("BH1750 read error: I2C communication failed.");
    } else {
        data.valid = true;
        data.lux = luxLevel;
    }

    return data;
}