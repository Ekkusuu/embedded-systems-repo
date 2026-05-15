#include "L293dFanActuator.h"

#include <Arduino.h>

#include "config/AppConfig.h"

namespace {

uint8_t percent_to_pwm(uint8_t percent) {
    return static_cast<uint8_t>((static_cast<uint16_t>(percent) * 255U) / 100U);
}

}  // namespace

L293dFanActuator::L293dFanActuator(uint8_t in1Pin, uint8_t in2Pin, uint8_t enPin)
    : _in1Pin(in1Pin),
      _in2Pin(in2Pin),
      _enPin(enPin),
      _targetPercent(0),
      _speedPercent(0),
      _lastRampMs(0) {}

void L293dFanActuator::begin() {
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
    pinMode(_enPin, OUTPUT);
    digitalWrite(_in1Pin, HIGH);
    digitalWrite(_in2Pin, LOW);
    applyOutputs(0);
}

void L293dFanActuator::setTargetPercent(uint8_t percent) {
    if (percent > 100U) {
        percent = 100U;
    }
    _targetPercent = percent;
}

void L293dFanActuator::tick(uint32_t nowMs) {
    if (nowMs - _lastRampMs < AppConfig::FanRampPeriodMs) {
        return;
    }
    _lastRampMs = nowMs;

    if (_speedPercent < _targetPercent) {
        const uint16_t next = static_cast<uint16_t>(_speedPercent) + AppConfig::FanRampStepPercent;
        _speedPercent = static_cast<uint8_t>(next > _targetPercent ? _targetPercent : next);
    } else if (_speedPercent > _targetPercent) {
        const uint8_t delta = _speedPercent - _targetPercent;
        _speedPercent -= (delta > AppConfig::FanRampStepPercent) ? AppConfig::FanRampStepPercent : delta;
    }

    applyOutputs(percent_to_pwm(_speedPercent));
}

uint8_t L293dFanActuator::targetPercent() const {
    return _targetPercent;
}

uint8_t L293dFanActuator::speedPercent() const {
    return _speedPercent;
}

uint8_t L293dFanActuator::speedPwm() const {
    return percent_to_pwm(_speedPercent);
}

void L293dFanActuator::applyOutputs(uint8_t pwmValue) {
    if (pwmValue == 0) {
        analogWrite(_enPin, 0);
        digitalWrite(_in1Pin, LOW);
        digitalWrite(_in2Pin, LOW);
        return;
    }

    digitalWrite(_in1Pin, HIGH);
    digitalWrite(_in2Pin, LOW);
    analogWrite(_enPin, pwmValue);
}
