#pragma once

#include <stdint.h>

class Stepper28BYJ48Actuator {
public:
    Stepper28BYJ48Actuator(uint8_t in1Pin, uint8_t in2Pin, uint8_t in3Pin, uint8_t in4Pin);

    void begin();
    void setSpeedPercent(uint8_t percent);
    void tick(uint32_t nowMicros);

    uint8_t getSpeedPercent() const;
    uint8_t getSpeedRpm() const;
    uint16_t getPositionStep() const;

private:
    void applyPhase(uint8_t phaseIndex);
    void releaseCoils();

    static constexpr uint8_t PhaseCount = 4;
    static constexpr uint16_t StepsPerRevolution = 2048;
    static constexpr uint8_t MaxRpm = 60;

    uint8_t _pins[4];
    uint8_t _speedPercent = 0;
    uint8_t _phaseIndex = 0;
    uint32_t _lastStepMicros = 0;
    uint32_t _stepIntervalMicros = 0;
    uint16_t _positionStep = 0;
};
