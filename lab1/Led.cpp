#include <Arduino.h>
#include "Led.h"

Led::Led(uint8_t pin) : _pin(pin), _state(false) {}

void Led::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    _state = false;
}

void Led::turnOn() {
    digitalWrite(_pin, HIGH);
    _state = true;
}

void Led::turnOff() {
    digitalWrite(_pin, LOW);
    _state = false;
}

bool Led::getState() const {
    return _state;
}