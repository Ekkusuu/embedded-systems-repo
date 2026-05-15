#pragma once

#include <stdint.h>

class L293dFanActuator {
public:
    L293dFanActuator(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin);

    void begin();
    void setTargetPercent(uint8_t percent);
    void tick(uint32_t nowMs);
    uint8_t targetPercent() const;
    uint8_t speedPercent() const;
    uint8_t speedPwm() const;

private:
    void applyOutputs(uint8_t pwmValue);

    uint8_t _in1Pin;
    uint8_t _in2Pin;
    uint8_t _enPin;
    uint8_t _targetPercent;
    uint8_t _speedPercent;
    uint32_t _lastRampMs;
};
