#pragma once

#include <Arduino.h>

class LedController {
public:
  LedController(uint8_t greenPin, uint8_t redPin);

  void begin();
  void showAccessGranted();
  void showAccessDenied();
  void clear();

private:
  uint8_t greenLedPin;
  uint8_t redLedPin;
};
