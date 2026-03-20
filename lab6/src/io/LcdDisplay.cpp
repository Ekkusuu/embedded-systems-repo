#include "LcdDisplay.h"

#include <Arduino.h>
#include <stdio.h>
#include <string.h>

LcdDisplay::LcdDisplay(uint8_t rsPin,
                       uint8_t enPin,
                       uint8_t d4Pin,
                       uint8_t d5Pin,
                       uint8_t d6Pin,
                       uint8_t d7Pin)
    : _lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin) {}

void LcdDisplay::begin() {
    _lcd.begin(16, 2);
    _lcd.clear();
    _lcd.setCursor(0, 0);
    _lcd.print("Lab6 Part2");
    _lcd.setCursor(0, 1);
    _lcd.print("Conditioning");
}

void LcdDisplay::showStatus(float ntcFilteredC,
                            float dhtFilteredC,
                            bool dhtValid,
                            bool alertActive,
                            float thresholdHighC) {
    char ntcBuf[8];
    char dhtBuf[8];
    char thresholdBuf[8];
    char line0[17];
    char line1[17];

    dtostrf(ntcFilteredC, 4, 1, ntcBuf);
    dtostrf(thresholdHighC, 4, 1, thresholdBuf);

    if (dhtValid) {
        dtostrf(dhtFilteredC, 4, 1, dhtBuf);
    } else {
        snprintf(dhtBuf, sizeof(dhtBuf), "n/a ");
    }

    snprintf(line0, sizeof(line0), "N:%s D:%s", ntcBuf, dhtBuf);
    snprintf(line1, sizeof(line1), "AVG TH:%s%c", thresholdBuf, alertActive ? '!' : ' ');

    for (uint8_t i = strlen(line0); i < 16; ++i) {
        line0[i] = ' ';
    }
    line0[16] = '\0';

    for (uint8_t i = strlen(line1); i < 16; ++i) {
        line1[i] = ' ';
    }
    line1[16] = '\0';

    _lcd.setCursor(0, 0);
    _lcd.print(line0);
    _lcd.setCursor(0, 1);
    _lcd.print(line1);
}
