#pragma once

#include <stdint.h>

class BinaryActuator {
public:
    explicit BinaryActuator(uint8_t pin);

    void begin();
    void setState(bool enabled);
    bool getState() const;

private:
    uint8_t _pin;
    bool _state = false;
};
