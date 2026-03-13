#include "LcdDisplay.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
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
    _lcd.print("Lab5 Monitor");
    _lcd.setCursor(0, 1);
    _lcd.print("Init...");
}

void LcdDisplay::showStatus(float ntcTempC,
                            float dhtTempC,
                            bool dhtValid,
                            bool alertActive,
                            float thresholdHighC,
                            float thresholdLowC) {
    char ntcBuf[8];
    char dhtBuf[8];
    char highBuf[8];
    char lowBuf[8];
    char dhtOut[6];
    char line0[17];
    char line1[17];

    dtostrf(ntcTempC, 5, 1, ntcBuf);
    if (dhtValid) {
        dtostrf(dhtTempC, 5, 1, dhtBuf);
    }

    dtostrf(thresholdHighC, 5, 1, highBuf);
    dtostrf(thresholdLowC, 5, 1, lowBuf);

    if (dhtValid) {
        snprintf(dhtOut, sizeof(dhtOut), "%s", dhtBuf);
    } else {
        snprintf(dhtOut, sizeof(dhtOut), " n/a ");
    }

    snprintf(line0, sizeof(line0), "N:%s D:%s", ntcBuf, dhtOut);
    snprintf(line1, sizeof(line1), "TH:%s/%s%c", highBuf, lowBuf,
             alertActive ? '!' : ' ');

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
