#include "BinaryThresholdFilter.h"

BinaryThresholdFilter::BinaryThresholdFilter(float highThresholdC,
                                             float lowThresholdC,
                                             uint8_t confirmSamples)
    : _highThresholdC(highThresholdC),
      _lowThresholdC(lowThresholdC),
      _confirmSamples(confirmSamples == 0 ? 1 : confirmSamples),
      _state{false, false, 0} {}

void BinaryThresholdFilter::reset(bool initialStableHigh) {
    _state.candidateHigh = initialStableHigh;
    _state.stableHigh = initialStableHigh;
    _state.pendingCounter = 0;
}

void BinaryThresholdFilter::setThresholds(float highThresholdC, float lowThresholdC) {
    _highThresholdC = highThresholdC;
    _lowThresholdC = lowThresholdC;
}

ThresholdState BinaryThresholdFilter::update(float valueC) {
    if (_state.candidateHigh) {
        if (valueC <= _lowThresholdC) {
            _state.candidateHigh = false;
        }
    } else {
        if (valueC >= _highThresholdC) {
            _state.candidateHigh = true;
        }
    }

    if (_state.candidateHigh == _state.stableHigh) {
        _state.pendingCounter = 0;
    } else {
        if (_state.pendingCounter < _confirmSamples) {
            ++_state.pendingCounter;
        }
        if (_state.pendingCounter >= _confirmSamples) {
            _state.stableHigh = _state.candidateHigh;
            _state.pendingCounter = 0;
        }
    }

    return _state;
}
