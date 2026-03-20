#pragma once

#include <LiquidCrystal.h>
#include <stdint.h>

class LcdDisplay {
public:
    LcdDisplay(uint8_t rsPin,
               uint8_t enPin,
               uint8_t d4Pin,
               uint8_t d5Pin,
               uint8_t d6Pin,
               uint8_t d7Pin);

    void begin();
    void showStatus(float ntcFilteredC,
                    float dhtFilteredC,
                    bool dhtValid,
                    bool alertActive,
                    float thresholdHighC);

private:
    LiquidCrystal _lcd;
};
