#include "LedController.h"

LedController::LedController(uint8_t greenPin, uint8_t redPin)
    : greenLedPin(greenPin), redLedPin(redPin) {}

void LedController::begin() {
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  clear();
}

void LedController::showAccessGranted() {
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
}

void LedController::showAccessDenied() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
}

void LedController::clear() {
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);
}
