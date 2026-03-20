#pragma once

#include <stdint.h>

class LedController {
public:
    LedController(uint8_t greenPin, uint8_t redPin);

    void begin();
    void setAlert(bool alert);

private:
    uint8_t _greenPin;
    uint8_t _redPin;
};
