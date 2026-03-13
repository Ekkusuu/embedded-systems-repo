#include "LedController.h"
#include <Arduino.h>

LedController::LedController(uint8_t greenPin, uint8_t redPin)
    : _greenPin(greenPin), _redPin(redPin) {}

void LedController::begin() {
    pinMode(_greenPin, OUTPUT);
    pinMode(_redPin,   OUTPUT);
    // Default: system OK
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_redPin,   LOW);
}

void LedController::setAlert(bool alert) {
    digitalWrite(_greenPin, alert ? LOW  : HIGH);
    digitalWrite(_redPin,   alert ? HIGH : LOW);
}
