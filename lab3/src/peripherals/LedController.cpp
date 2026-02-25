#include "LedController.h"

LedController::LedController(uint8_t greenPin, uint8_t redPin, uint8_t yellowPin)
    : greenPin_(greenPin), redPin_(redPin), yellowPin_(yellowPin) {}

void LedController::begin() {
    pinMode(greenPin_,  OUTPUT);
    pinMode(redPin_,    OUTPUT);
    pinMode(yellowPin_, OUTPUT);
    clearIndicator();
    setYellow(false);
}

void LedController::setGreen(bool on) {
    digitalWrite(greenPin_, on ? HIGH : LOW);
}

void LedController::setRed(bool on) {
    digitalWrite(redPin_, on ? HIGH : LOW);
}

void LedController::setYellow(bool on) {
    digitalWrite(yellowPin_, on ? HIGH : LOW);
}

void LedController::indicateShortPress() {
    digitalWrite(greenPin_, HIGH);
    digitalWrite(redPin_,   LOW);
}

void LedController::indicateLongPress() {
    digitalWrite(redPin_,   HIGH);
    digitalWrite(greenPin_, LOW);
}

void LedController::clearIndicator() {
    digitalWrite(greenPin_, LOW);
    digitalWrite(redPin_,   LOW);
}
