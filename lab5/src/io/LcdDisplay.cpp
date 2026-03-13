#include "LcdDisplay.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

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

void LcdDisplay::showStatus(float ntcTempC, float dhtTempC, bool dhtValid, bool alertActive) {
    char ntcBuf[8];
    char dhtBuf[8];

    dtostrf(ntcTempC, 5, 1, ntcBuf);
    if (dhtValid) {
        dtostrf(dhtTempC, 5, 1, dhtBuf);
    }

    _lcd.setCursor(0, 0);
    _lcd.print("N:");
    _lcd.print(ntcBuf);
    _lcd.print(" D:");
    if (dhtValid) {
        _lcd.print(dhtBuf);
    } else {
        _lcd.print(" n/a ");
    }

    _lcd.setCursor(0, 1);
    _lcd.print("ALERT:");
    _lcd.print(alertActive ? "YES" : " NO");
    _lcd.print("         ");
}
