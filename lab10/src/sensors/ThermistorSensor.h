#pragma once

#include <stdint.h>

struct TemperatureSample {
    bool valid;
    uint16_t raw;
    float voltage;
    float resistanceOhm;
    float celsius;
};

class ThermistorSensor {
public:
    explicit ThermistorSensor(uint8_t analogPin);

    void begin() const;
    TemperatureSample readTemperature() const;

private:
    uint8_t _analogPin;
};
