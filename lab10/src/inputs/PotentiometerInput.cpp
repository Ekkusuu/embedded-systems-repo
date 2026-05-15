#include "PotentiometerInput.h"

#include <Arduino.h>

PotentiometerInput::PotentiometerInput(uint8_t pin, float minValue, float maxValue)
    : _pin(pin), _minValue(minValue), _maxValue(maxValue) {}

void PotentiometerInput::begin() const {
    pinMode(_pin, INPUT);
}

float PotentiometerInput::readMappedValue() const {
    const int raw = analogRead(_pin);
    const float ratio = static_cast<float>(raw) / 1023.0f;
    return _minValue + ratio * (_maxValue - _minValue);
}
