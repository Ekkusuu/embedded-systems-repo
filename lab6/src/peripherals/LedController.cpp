#include "LedController.h"

#include <Arduino.h>

#include "config/AppConfig.h"

LedController::LedController(uint8_t greenPin, uint8_t redPin, uint8_t bluePin)
    : _greenPin(greenPin), _redPin(redPin), _bluePin(bluePin) {}

void LedController::begin() {
    pinMode(_greenPin, OUTPUT);
    pinMode(_redPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_redPin, LOW);
    digitalWrite(_bluePin, HIGH);
}

void LedController::setAlert(bool alert) {
    digitalWrite(_greenPin, alert ? LOW : HIGH);
    digitalWrite(_redPin, alert ? HIGH : LOW);
}

void LedController::setStability(bool stable, uint32_t nowMs) {
    if (stable) {
        digitalWrite(_bluePin, HIGH);
        return;
    }

    const bool blinkOn = ((nowMs / AppConfig::StabilityBlinkPeriodMs) % 2U) == 0U;
    digitalWrite(_bluePin, blinkOn ? HIGH : LOW);
}
