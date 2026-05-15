#pragma once

#include <stdint.h>

class ButtonLedFsmApp {
public:
    void begin();
    void tick(uint32_t nowMs);

private:
    enum class LedState : uint8_t {
        Off,
        On,
    };

    void pollButton(uint32_t nowMs);
    void toggleState();
    void applyOutput() const;
    void updateLcd() const;
    void reportState(uint32_t nowMs, bool forced);
    const char* stateName() const;

    LedState _state = LedState::Off;
    bool _lastReading = false;
    bool _debouncedPressed = false;
    uint32_t _lastDebounceMs = 0;
    uint32_t _lastReportMs = 0;
};
