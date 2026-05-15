#include "OnOffHysteresisController.h"

OnOffHysteresisController::OnOffHysteresisController(float setpoint, float halfBand)
    : _setpoint(setpoint), _halfBand(halfBand), _output(false) {}

void OnOffHysteresisController::setSetpoint(float setpoint) {
    _setpoint = setpoint;
}

float OnOffHysteresisController::setpoint() const {
    return _setpoint;
}

bool OnOffHysteresisController::update(float measuredValue) {
    const float onThreshold = thresholdOn();
    const float offThreshold = thresholdOff();

    if (measuredValue <= onThreshold) {
        _output = true;
    } else if (measuredValue >= offThreshold) {
        _output = false;
    }

    return _output;
}

bool OnOffHysteresisController::output() const {
    return _output;
}

float OnOffHysteresisController::thresholdOn() const {
    return _setpoint - _halfBand;
}

float OnOffHysteresisController::thresholdOff() const {
    return _setpoint + _halfBand;
}
