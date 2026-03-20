#include "AnalogConditioner.h"

#include "config/AppConfig.h"

namespace {

float clamp_value(float value, float minimum, float maximum) {
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

void sort3(float* values) {
    if (values[0] > values[1]) {
        const float tmp = values[0];
        values[0] = values[1];
        values[1] = tmp;
    }
    if (values[1] > values[2]) {
        const float tmp = values[1];
        values[1] = values[2];
        values[2] = tmp;
    }
    if (values[0] > values[1]) {
        const float tmp = values[0];
        values[0] = values[1];
        values[1] = tmp;
    }
}

}  // namespace

AnalogConditioner::AnalogConditioner(float minimum, float maximum)
    : _minimum(minimum),
      _maximum(maximum),
      _medianBuffer{0.0f, 0.0f, 0.0f},
      _weightedBuffer{0.0f, 0.0f, 0.0f, 0.0f},
      _medianCount(0),
      _weightedCount(0),
      _medianIndex(0),
      _weightedIndex(0),
      _initialized(false),
      _lastMedian(0.0f),
      _lastWeighted(0.0f) {}

void AnalogConditioner::reset() {
    _medianCount = 0;
    _weightedCount = 0;
    _medianIndex = 0;
    _weightedIndex = 0;
    _initialized = false;
    _lastMedian = 0.0f;
    _lastWeighted = 0.0f;
}

AnalogConditionedValue AnalogConditioner::update(float rawValue) {
    AnalogConditionedValue value{};
    value.raw = rawValue;
    value.saturated = clamp_value(rawValue, _minimum, _maximum);
    value.median = computeMedian(value.saturated);
    value.weighted = computeWeighted(value.median);
    value.initialized = _initialized;
    return value;
}

float AnalogConditioner::computeMedian(float value) {
    _medianBuffer[_medianIndex] = value;
    _medianIndex = static_cast<uint8_t>((_medianIndex + 1U) % 3U);
    if (_medianCount < 3U) {
        ++_medianCount;
    }

    if (_medianCount < 3U) {
        _lastMedian = value;
        return _lastMedian;
    }

    float copy[3] = {_medianBuffer[0], _medianBuffer[1], _medianBuffer[2]};
    sort3(copy);
    _lastMedian = copy[1];
    _initialized = true;
    return _lastMedian;
}

float AnalogConditioner::computeWeighted(float value) {
    _weightedBuffer[_weightedIndex] = value;
    _weightedIndex = static_cast<uint8_t>((_weightedIndex + 1U) % 4U);
    if (_weightedCount < 4U) {
        ++_weightedCount;
    }

    if (_weightedCount < 4U) {
        _lastWeighted = value;
        return _lastWeighted;
    }

    float weightedSum = 0.0f;
    uint16_t weightTotal = 0;

    for (uint8_t age = 0; age < 4U; ++age) {
        const int8_t index = static_cast<int8_t>(_weightedIndex) - 1 - static_cast<int8_t>(age);
        const uint8_t bufferIndex = static_cast<uint8_t>((index + 4) % 4);
        const uint8_t weight = AppConfig::WeightedCoefficients[age];
        weightedSum += _weightedBuffer[bufferIndex] * static_cast<float>(weight);
        weightTotal += weight;
    }

    _lastWeighted = static_cast<float>(weightedSum) / static_cast<float>(weightTotal);
    return _lastWeighted;
}
