#include "BinaryActuator.h"

#include <Arduino.h>

BinaryActuator::BinaryActuator(uint8_t pin) : _pin(pin) {}

void BinaryActuator::begin() {
    pinMode(_pin, OUTPUT);
    setState(false);
}

void BinaryActuator::setState(bool enabled) {
    _state = enabled;
    digitalWrite(_pin, _state ? HIGH : LOW);
}

bool BinaryActuator::getState() const {
    return _state;
}
