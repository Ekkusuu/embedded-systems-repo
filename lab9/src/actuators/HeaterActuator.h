#pragma once

#include <stdint.h>

class HeaterActuator {
public:
    explicit HeaterActuator(uint8_t pin);

    void begin();
    void setState(bool on);
    bool state() const;

private:
    uint8_t _pin;
    bool _state;
};
