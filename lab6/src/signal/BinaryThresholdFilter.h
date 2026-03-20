#pragma once

#include <stdint.h>

struct ThresholdState {
    bool candidateHigh;
    bool stableHigh;
    uint8_t pendingCounter;
};

class BinaryThresholdFilter {
public:
    BinaryThresholdFilter(float highThresholdC, float lowThresholdC, uint8_t confirmSamples);

    void reset(bool initialStableHigh = false);
    void setThresholds(float highThresholdC, float lowThresholdC);
    ThresholdState update(float valueC);

private:
    float _highThresholdC;
    float _lowThresholdC;
    uint8_t _confirmSamples;
    ThresholdState _state;
};
