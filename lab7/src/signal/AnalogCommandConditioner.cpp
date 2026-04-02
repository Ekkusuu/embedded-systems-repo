#include "AnalogCommandConditioner.h"

#include <math.h>

AnalogCommandConditioner::AnalogCommandConditioner(int minPercent,
                                                   int maxPercent,
                                                   float filterAlpha,
                                                   float rampStepPercent,
                                                   uint8_t highAlertPercent)
    : _minPercent(minPercent),
      _maxPercent(maxPercent),
      _filterAlpha(filterAlpha),
      _rampStepPercent(rampStepPercent),
      _highAlertPercent(highAlertPercent) {}

void AnalogCommandConditioner::setTargetPercent(int requestedPercent) {
    _requestedPercent = requestedPercent;
    _saturatedTargetPercent = clamp(requestedPercent);
    _saturationActive = (_requestedPercent != _saturatedTargetPercent);
}

void AnalogCommandConditioner::tick() {
    const float target = static_cast<float>(_saturatedTargetPercent);
    _filteredPercent = (_filterAlpha * target) + ((1.0f - _filterAlpha) * _filteredPercent);

    const float delta = _filteredPercent - _outputPercent;

    if (delta > _rampStepPercent) {
        _outputPercent += _rampStepPercent;
    } else if (delta < -_rampStepPercent) {
        _outputPercent -= _rampStepPercent;
    } else {
        _outputPercent = _filteredPercent;
    }

    if (_outputPercent < static_cast<float>(_minPercent)) {
        _outputPercent = static_cast<float>(_minPercent);
    }

    if (_outputPercent > static_cast<float>(_maxPercent)) {
        _outputPercent = static_cast<float>(_maxPercent);
    }

    _highAlertActive = (getOutputPercent() >= _highAlertPercent);
}

int AnalogCommandConditioner::getRequestedPercent() const {
    return _requestedPercent;
}

int AnalogCommandConditioner::getSaturatedTargetPercent() const {
    return _saturatedTargetPercent;
}

uint8_t AnalogCommandConditioner::getOutputPercent() const {
    return static_cast<uint8_t>(lroundf(_outputPercent));
}

uint8_t AnalogCommandConditioner::getPwmValue() const {
    const long scaled = static_cast<long>(getOutputPercent()) * 255L;
    return static_cast<uint8_t>(scaled / 100L);
}

bool AnalogCommandConditioner::isSaturationActive() const {
    return _saturationActive;
}

bool AnalogCommandConditioner::isHighAlertActive() const {
    return _highAlertActive;
}

int AnalogCommandConditioner::clamp(int value) const {
    if (value < _minPercent) {
        return _minPercent;
    }

    if (value > _maxPercent) {
        return _maxPercent;
    }

    return value;
}
