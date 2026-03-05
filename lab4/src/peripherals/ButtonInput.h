#pragma once
#include <Arduino.h>

// ============================================================
// ButtonInput – ECAL-level driver for a single debounced push-button.
//
// Wiring:   Arduino pin → button → GND   (INPUT_PULLUP: pressed = LOW)
//
// Design:   Non-blocking state machine; call update() each task tick.
//           wasReleased() returns true for exactly one tick after the
//           button is released and the press duration has been recorded.
// ============================================================
class ButtonInput {
public:
    explicit ButtonInput(uint8_t pin);

    void begin();

    // Advance the debounce FSM. Call once per task tick with current ms.
    void update(uint32_t nowMs);

    // Returns true for exactly one tick after a confirmed release.
    bool     wasReleased();
    uint32_t getLastPressDurationMs() const;

private:
    enum class State : uint8_t {
        IDLE,
        DEBOUNCING_PRESS,
        HELD,
        DEBOUNCING_RELEASE
    };

    uint8_t  pin_;
    State    state_;
    uint32_t debounceStartMs_;
    uint32_t pressStartMs_;
    uint32_t lastPressDuration_;
    bool     releasedFlag_;
};
