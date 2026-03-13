#pragma once
#include <stdint.h>

// ============================================================
// LedController – manages the green (OK) and red (alert) LEDs.
// ============================================================

class LedController {
public:
    LedController(uint8_t greenPin, uint8_t redPin);

    void begin();

    // Drive both LEDs atomically based on alert state.
    // alert=false → green ON, red OFF
    // alert=true  → green OFF, red ON
    void setAlert(bool alert);

private:
    uint8_t _greenPin;
    uint8_t _redPin;
};
