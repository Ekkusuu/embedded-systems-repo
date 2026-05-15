#pragma once

#include <stdint.h>

class PotentiometerInput {
public:
    PotentiometerInput(uint8_t pin, float minValue, float maxValue);

    void begin() const;
    float readMappedValue() const;

private:
    uint8_t _pin;
    float _minValue;
    float _maxValue;
};
