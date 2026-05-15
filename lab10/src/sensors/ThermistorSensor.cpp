#include "ThermistorSensor.h"

#include <Arduino.h>
#include <math.h>

#include "config/AppConfig.h"

ThermistorSensor::ThermistorSensor(uint8_t analogPin) : _analogPin(analogPin) {}

void ThermistorSensor::begin() const {
    pinMode(_analogPin, INPUT);
}

TemperatureSample ThermistorSensor::readTemperature() const {
    TemperatureSample sample{};
    sample.valid = false;
    sample.raw = static_cast<uint16_t>(analogRead(_analogPin));

    if (sample.raw == 0 || sample.raw >= 1023) {
        return sample;
    }

    sample.voltage = (static_cast<float>(sample.raw) * AppConfig::AdcVref) / 1023.0f;
    sample.resistanceOhm = AppConfig::ThermistorSeriesResistorOhm *
                           (AppConfig::AdcVref - sample.voltage) /
                           sample.voltage;

    const float ratio = sample.resistanceOhm / AppConfig::ThermistorNominalOhm;
    const float inverseKelvin =
        (1.0f / AppConfig::ThermistorNominalKelvin) + (log(ratio) / AppConfig::ThermistorBeta);

    if (inverseKelvin <= 0.0f || !isfinite(inverseKelvin)) {
        return sample;
    }

    sample.celsius = (1.0f / inverseKelvin) - 273.15f;
    sample.valid = isfinite(sample.celsius);
    return sample;
}
