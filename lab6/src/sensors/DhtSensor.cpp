#include "DhtSensor.h"

#include <Arduino.h>
#include <math.h>

DhtSensor::DhtSensor(uint8_t dataPin)
    : _dht(dataPin, DHT11), _lastTemperatureC(0.0f), _hasValue(false) {}

void DhtSensor::begin() {
    _dht.begin();
}

DhtSample DhtSensor::readSample() {
    DhtSample sample{};
    const float t = _dht.readTemperature();

    if (!isnan(t)) {
        _lastTemperatureC = t;
        _hasValue = true;
        sample.valid = true;
        sample.fresh = true;
        sample.temperatureC = t;
    } else {
        sample.valid = _hasValue;
        sample.fresh = false;
        sample.temperatureC = _lastTemperatureC;
    }

    return sample;
}
