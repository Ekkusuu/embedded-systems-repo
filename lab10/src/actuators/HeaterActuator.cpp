#include "HeaterActuator.h"

#include <Arduino.h>

HeaterActuator::HeaterActuator(uint8_t pin) : _pin(pin), _state(false) {}

void HeaterActuator::begin() {
    pinMode(_pin, OUTPUT);
    setState(false);
}

void HeaterActuator::setState(bool on) {
    _state = on;
    digitalWrite(_pin, _state ? HIGH : LOW);
}

bool HeaterActuator::state() const {
    return _state;
}
