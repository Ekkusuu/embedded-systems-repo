#include "BuzzerController.h"

#include <Arduino.h>

BuzzerController::BuzzerController(uint8_t pin) : _pin(pin) {}

void BuzzerController::begin() {
    pinMode(_pin, OUTPUT);
    noTone(_pin);
}

void BuzzerController::setAlert(bool alert) {
    if (alert) {
        tone(_pin, ToneHz);
    } else {
        noTone(_pin);
    }
}
