#pragma once
#include <stdint.h>

// ============================================================
// BuzzerController – drives a passive buzzer via tone().
// setAlert(true)  → buzzer ON at kToneHz
// setAlert(false) → buzzer OFF
// ============================================================

class BuzzerController {
public:
    explicit BuzzerController(uint8_t pin);

    void begin();
    void setAlert(bool alert);

private:
    uint8_t _pin;
    static constexpr uint16_t kToneHz = 1000;
};
