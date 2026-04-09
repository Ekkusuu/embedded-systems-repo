#pragma once

#include <stdint.h>

class AnalogCommandConditioner {
public:
    AnalogCommandConditioner(int minPercent,
                             int maxPercent,
                             float filterAlpha,
                             float rampStepPercent,
                             uint8_t highAlertPercent);

    void setTargetPercent(int requestedPercent);
    void tick();

    int getRequestedPercent() const;
    int getSaturatedTargetPercent() const;
    int getOutputPercent() const;
    bool isSaturationActive() const;
    bool isHighAlertActive() const;

private:
    int clamp(int value) const;

    const int _minPercent;
    const int _maxPercent;
    const float _filterAlpha;
    const float _rampStepPercent;
    const uint8_t _highAlertPercent;

    int _requestedPercent = 0;
    int _saturatedTargetPercent = 0;
    float _filteredPercent = 0.0f;
    float _outputPercent = 0.0f;
    bool _saturationActive = false;
    bool _highAlertActive = false;
};
