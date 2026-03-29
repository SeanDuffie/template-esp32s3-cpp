#include "ds18B20_temp.h"
#include "debug.h"

DS18B20Sensor::DS18B20Sensor(OneWire& oneWireBus, uint8_t sensorIndex) 
    : _sensors(&oneWireBus), _index(sensorIndex) {}

bool DS18B20Sensor::begin() {
    _sensors.begin();

    // Verify a sensor actually exists at this position on the bus
    if (_sensors.getDeviceCount() <= _index) {
        debug_printf("DS18B20 initialization failed: No sensor found at index %d.\n", _index);
        return false;
    }

    // Set to 12-bit resolution for maximum precision (0.0625°C increments)
    _sensors.setResolution(12);

    debug_printf("DS18B20 initialized successfully at index %d.\n", _index);
    return true;
}

TempData DS18B20Sensor::readData() {
    TempData data;

    // Send the global command to all sensors on the bus to take a reading
    _sensors.requestTemperatures();

    // Fetch the specific sensor's data
    float tempC = _sensors.getTempCByIndex(_index);

    // DallasTemperature returns DEVICE_DISCONNECTED_C (-127.0) if the hardware fails
    if (tempC <= -127.0) {
        data.valid = false;
        data.tempC = 0.0;
        data.tempF = 0.0;
        debug_printf("DS18B20 hardware error: Sensor dropped at index %d.\n", _index);
    } else {
        data.valid = true;
        data.tempC = tempC;
        data.tempF = DallasTemperature::toFahrenheit(tempC);
    }

    return data;
}