#include "ButtonInput.h"
#include "../config/AppConfig.h"

ButtonInput::ButtonInput(uint8_t pin)
    : pin_(pin),
      state_(State::IDLE),
      debounceStartMs_(0),
      pressStartMs_(0),
      lastPressDuration_(0),
      releasedFlag_(false) {}

void ButtonInput::begin() {
    pinMode(pin_, INPUT_PULLUP);
}

void ButtonInput::update(uint32_t nowMs) {
    releasedFlag_ = false;  // cleared every tick; may be set once below
    bool raw = (digitalRead(pin_) == LOW);  // LOW = pressed (pull-up wiring)

    switch (state_) {
        case State::IDLE:
            if (raw) {
                state_           = State::DEBOUNCING_PRESS;
                debounceStartMs_ = nowMs;
            }
            break;

        case State::DEBOUNCING_PRESS:
            if (!raw) {
                state_ = State::IDLE;  // signal noise – reset
            } else if ((nowMs - debounceStartMs_) >= AppConfig::DebounceMs) {
                // Stable press confirmed
                state_        = State::HELD;
                pressStartMs_ = nowMs;
            }
            break;

        case State::HELD:
            if (!raw) {
                state_           = State::DEBOUNCING_RELEASE;
                debounceStartMs_ = nowMs;
            }
            break;

        case State::DEBOUNCING_RELEASE:
            if (raw) {
                state_ = State::HELD;  // signal noise – still held
            } else if ((nowMs - debounceStartMs_) >= AppConfig::DebounceMs) {
                // Release confirmed – record duration
                lastPressDuration_ = debounceStartMs_ - pressStartMs_;
                releasedFlag_      = true;
                state_             = State::IDLE;
            }
            break;
    }
}

bool ButtonInput::wasReleased() {
    return releasedFlag_;
}

uint32_t ButtonInput::getLastPressDurationMs() const {
    return lastPressDuration_;
}
