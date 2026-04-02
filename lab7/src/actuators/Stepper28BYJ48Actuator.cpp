#include "Stepper28BYJ48Actuator.h"

#include <Arduino.h>

namespace {

// Full-step sequence for 28BYJ-48 through a ULN2003 driver.
constexpr uint8_t kFullStepTable[4][4] = {
    {1, 0, 0, 1},
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
};

}  // namespace

Stepper28BYJ48Actuator::Stepper28BYJ48Actuator(uint8_t in1Pin,
                                               uint8_t in2Pin,
                                               uint8_t in3Pin,
                                               uint8_t in4Pin)
    : _pins{in1Pin, in2Pin, in3Pin, in4Pin} {}

void Stepper28BYJ48Actuator::begin() {
    for (uint8_t pin : _pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void Stepper28BYJ48Actuator::setSpeedPercent(uint8_t percent) {
    _speedPercent = percent;

    if (_speedPercent == 0) {
        _stepIntervalMicros = 0;
        releaseCoils();
        return;
    }

    const uint32_t rpm = static_cast<uint32_t>((static_cast<uint16_t>(_speedPercent) * MaxRpm) / 100U);
    const uint32_t effectiveRpm = rpm == 0 ? 1U : rpm;
    _stepIntervalMicros = (60UL * 1000UL * 1000UL) / (StepsPerRevolution * effectiveRpm);
}

void Stepper28BYJ48Actuator::tick(uint32_t nowMicros) {
    if (_stepIntervalMicros == 0) {
        return;
    }

    if ((nowMicros - _lastStepMicros) < _stepIntervalMicros) {
        return;
    }

    _lastStepMicros = nowMicros;
    _phaseIndex = static_cast<uint8_t>((_phaseIndex + 1U) % PhaseCount);
    _positionStep = static_cast<uint16_t>((_positionStep + 1U) % StepsPerRevolution);
    applyPhase(_phaseIndex);
}

uint8_t Stepper28BYJ48Actuator::getSpeedPercent() const {
    return _speedPercent;
}

uint8_t Stepper28BYJ48Actuator::getSpeedRpm() const {
    return static_cast<uint8_t>((static_cast<uint16_t>(_speedPercent) * MaxRpm) / 100U);
}

uint16_t Stepper28BYJ48Actuator::getPositionStep() const {
    return _positionStep;
}

void Stepper28BYJ48Actuator::applyPhase(uint8_t phaseIndex) {
    for (uint8_t i = 0; i < 4; ++i) {
        digitalWrite(_pins[i], kFullStepTable[phaseIndex][i] ? HIGH : LOW);
    }
}

void Stepper28BYJ48Actuator::releaseCoils() {
    for (uint8_t pin : _pins) {
        digitalWrite(pin, LOW);
    }
}
