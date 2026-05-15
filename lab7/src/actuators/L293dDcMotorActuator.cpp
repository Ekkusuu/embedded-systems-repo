#include "L293dDcMotorActuator.h"

#include <Arduino.h>
#include <stdlib.h>

L293dDcMotorActuator::L293dDcMotorActuator(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin)
    : _in1Pin(in1Pin), _in2Pin(in2Pin), _enPin(enPin) {}

void L293dDcMotorActuator::begin() {
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
    pinMode(_enPin, OUTPUT);
    setSpeedPercent(0);
    applyOutputs(0, true);
}

void L293dDcMotorActuator::setSpeedPercent(int percent) {
    if (percent > 100) {
        percent = 100;
    }
    if (percent < -100) {
        percent = -100;
    }

    _speedPercent = percent;
    _forward = (_speedPercent >= 0);

    const uint8_t magnitude = static_cast<uint8_t>(abs(_speedPercent));
    _targetPwm = static_cast<uint8_t>((static_cast<uint16_t>(magnitude) * 255U) / 100U);

    _activePwm = _targetPwm;
    applyOutputs(_activePwm, _forward);
}

void L293dDcMotorActuator::tick(uint32_t nowMs) {
    (void)nowMs;
}

int L293dDcMotorActuator::getSpeedPercent() const {
    return _speedPercent;
}

uint8_t L293dDcMotorActuator::getSpeedPwm() const {
    return _activePwm;
}

bool L293dDcMotorActuator::isForward() const {
    return _forward;
}

void L293dDcMotorActuator::applyOutputs(uint8_t pwmValue, bool forward) {
    if (pwmValue == 0) {
        analogWrite(_enPin, 0);
        digitalWrite(_in1Pin, LOW);
        digitalWrite(_in2Pin, LOW);
        return;
    }

    digitalWrite(_in1Pin, forward ? HIGH : LOW);
    digitalWrite(_in2Pin, forward ? LOW : HIGH);
    analogWrite(_enPin, pwmValue);
}
