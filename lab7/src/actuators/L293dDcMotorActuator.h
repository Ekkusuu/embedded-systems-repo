#pragma once

#include <stdint.h>

class L293dDcMotorActuator {
public:
    L293dDcMotorActuator(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin);

    void begin();
    void setSpeedPercent(int percent);
    void tick(uint32_t nowMs);

    int getSpeedPercent() const;
    uint8_t getSpeedPwm() const;
    bool isForward() const;

private:
    void applyOutputs(uint8_t pwmValue, bool forward);

    static constexpr uint8_t MinRunPwm = 110;
    static constexpr uint8_t KickstartPwm = 255;
    static constexpr uint16_t KickstartDurationMs = 120;

    uint8_t _in1Pin;
    uint8_t _in2Pin;
    uint8_t _enPin;
    int _speedPercent = 0;
    uint8_t _targetPwm = 0;
    uint8_t _activePwm = 0;
    bool _forward = true;
    bool _lastAppliedForward = true;
    uint32_t _kickstartUntilMs = 0;
};
