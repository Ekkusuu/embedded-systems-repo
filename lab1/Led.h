#ifndef LED_H
#define LED_H

#include <stdint.h>

class Led {
  private:
    uint8_t _pin;
    bool    _state;

  public:
    Led(uint8_t pin);
    void begin();
    void turnOn();
    void turnOff();
    bool getState() const;
};

#endif // LED_H