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
    void showPage0(bool binaryState,
                   bool binaryPending,
                   uint8_t stepperPercent,
                   uint8_t stepperRpm,
                   uint16_t stepperPosition);
    void showPage1(bool saturationAlert, bool highAlert, bool reportingEnabled);

private:
    void printPaddedLine(uint8_t row, const char* text);

    LiquidCrystal _lcd;
};
