#pragma once

#include <stdint.h>

class LedController {
public:
    LedController(uint8_t greenPin, uint8_t redPin, uint8_t bluePin);

    void begin();
    void setAlert(bool alert);
    void setStability(bool stable, uint32_t nowMs);

private:
    uint8_t _greenPin;
    uint8_t _redPin;
    uint8_t _bluePin;
};
