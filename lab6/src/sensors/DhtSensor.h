#pragma once

#include <DHT.h>
#include <stdint.h>

struct DhtSample {
    bool valid;
    bool fresh;
    float temperatureC;
};

class DhtSensor {
public:
    explicit DhtSensor(uint8_t dataPin);

    void begin();
    DhtSample readSample();

private:
    DHT _dht;
    float _lastTemperatureC;
    bool _hasValue;
};
