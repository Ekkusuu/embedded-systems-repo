#pragma once

#include <stdint.h>

class BuzzerController {
public:
    explicit BuzzerController(uint8_t pin);

    void begin();
    void setAlert(bool alert);

private:
    uint8_t _pin;
    static constexpr uint16_t ToneHz = 1000;
};
