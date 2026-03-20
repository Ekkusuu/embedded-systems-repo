#include "NtcSensor.h"

#include <Arduino.h>
#include <math.h>

#include "config/AppConfig.h"

NtcSensor::NtcSensor(uint8_t analogPin) : _analogPin(analogPin) {}

void NtcSensor::begin() const {
    pinMode(_analogPin, INPUT);
}

NtcSample NtcSensor::readSample() const {
    NtcSample sample{};

    sample.raw = static_cast<uint16_t>(analogRead(_analogPin));
    sample.voltage = (static_cast<float>(sample.raw) * AppConfig::AdcVref) / 1023.0f;

    const float denominator = AppConfig::AdcVref - sample.voltage;
    if (denominator <= 0.001f) {
        sample.resistanceOhm = 10000000.0f;
    } else {
        sample.resistanceOhm = AppConfig::NtcRFixedOhm * sample.voltage / denominator;
    }

    const float ratio = sample.resistanceOhm / AppConfig::NtcR25Ohm;
    const float inverseT = (1.0f / AppConfig::NtcT0Kelvin) + (log(ratio) / AppConfig::NtcBeta);
    const float temperatureKelvin = 1.0f / inverseT;
    sample.temperatureC = temperatureKelvin - 273.15f;

    return sample;
}
