#include "ds3231_rtc.h"
#include "debug.h"

DS3231Sensor::DS3231Sensor(TwoWire& wireBus) 
    : _wire(wireBus) {}

bool DS3231Sensor::begin() {
    // RTClib handles the 0x68 address internally. We just pass the bus pointer.
    if (!_rtc.begin(&_wire)) {
        debug_println("DS3231 initialization failed. Check wiring or address conflicts.");
        return false;
    }

    if (_rtc.lostPower()) {
        debug_println("RTC lost power! Time is invalid until synchronized.");
    } else {
        debug_println("DS3231 initialized successfully.");
    }

    return true;
}

void DS3231Sensor::adjustTime(uint32_t newTime) {
    _rtc.adjust(DateTime(newTime));
    debug_println("RTC time manually adjusted.");
}

RTCData DS3231Sensor::readData() {
    RTCData data;
    
    // Attempt to read the time
    DateTime now = _rtc.now();
    
    // A quick validation check. If I2C fails, RTClib usually returns a year like 2165 or 2000.
    if (now.year() < 2024 || now.year() > 2100) {
        data.valid = false;
        data.unixtime = 0;
        data.temperatureF = 0.0;
        data.lostPower = true;
        debug_println("DS3231 read error: Invalid year returned.");
    } else {
        data.valid = true;
        data.unixtime = now.unixtime();
        data.temperatureF = (_rtc.getTemperature() * 9.0 / 5.0) + 32.0;
        data.lostPower = _rtc.lostPower();
    }

    return data;
}