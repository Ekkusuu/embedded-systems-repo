#pragma once

#include <stdint.h>

struct AnalogConditionedValue {
    bool initialized;
    float raw;
    float saturated;
    float median;
    float weighted;
};

class AnalogConditioner {
public:
    AnalogConditioner(float minimum, float maximum);

    void reset();
    AnalogConditionedValue update(float rawValue);

private:
    float _minimum;
    float _maximum;
    float _medianBuffer[3];
    float _weightedBuffer[4];
    uint8_t _medianCount;
    uint8_t _weightedCount;
    uint8_t _medianIndex;
    uint8_t _weightedIndex;
    bool _initialized;
    float _lastMedian;
    float _lastWeighted;

    float computeMedian(float value);
    float computeWeighted(float value);
};
