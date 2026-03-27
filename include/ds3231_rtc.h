#pragma once
#include <Wire.h>
#include <RTClib.h>

struct RTCData {
    uint32_t unixtime;    // Seconds since Jan 1, 1970
    float temperatureF;   // Internal module temperature
    bool valid;           // False if module is missing or battery died
    bool lostPower;       // True if the coin cell died and time reset
};

class DS3231Sensor {
private:
    RTC_DS3231 _rtc;
    TwoWire& _wire;

public:
    DS3231Sensor(TwoWire& wireBus);
    
    bool begin();
    
    // Sets the RTC time to an explicit UNIX timestamp
    void adjustTime(uint32_t newTime); 
    
    RTCData readData();
};