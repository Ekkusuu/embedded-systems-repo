#pragma once

#include <stdint.h>

struct NtcSample {
    uint16_t raw;
    float voltage;
    float resistanceOhm;
    float temperatureC;
};

class NtcSensor {
public:
    explicit NtcSensor(uint8_t analogPin);

    void begin() const;
    NtcSample readSample() const;

private:
    uint8_t _analogPin;
};
