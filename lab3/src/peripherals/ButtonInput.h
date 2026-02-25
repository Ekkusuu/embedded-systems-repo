#pragma once
#include <Arduino.h>

// ---------------------------------------------------------------------------
// ButtonInput – ECAL-level driver for a single debounced push-button.
//
// Wiring:   Arduino pin → button → GND   (INPUT_PULLUP: pressed = LOW)
//
// Design:   Non-blocking state machine; call update() every scheduler tick.
//           wasReleased() returns true for exactly one tick after the button
//           has been released and the press duration has been recorded.
// ---------------------------------------------------------------------------
class ButtonInput {
public:
    explicit ButtonInput(uint8_t pin);

    void     begin();
    // Call once per task tick to advance the debounce state machine.
    void     update(uint32_t nowMs);

    // Returns true during the one tick after release; clears automatically.
    bool     wasReleased();
    uint32_t getLastPressDurationMs() const;

private:
    enum class State : uint8_t { IDLE, DEBOUNCING_PRESS, HELD, DEBOUNCING_RELEASE };

    uint8_t  pin_;
    State    state_;
    uint32_t debounceStartMs_;
    uint32_t pressStartMs_;
    uint32_t lastPressDuration_;
    bool     releasedFlag_;
};
