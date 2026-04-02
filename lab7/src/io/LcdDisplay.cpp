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
    printPaddedLine(0, "Lab 7 Actuator");
    printPaddedLine(1, "STDIO + LCD");
}

void LcdDisplay::showPage0(bool binaryState,
                           bool binaryPending,
                           uint8_t stepperPercent,
                           uint8_t stepperRpm,
                           uint16_t stepperPosition) {
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "R:%s%s S:%3u%%",
             binaryState ? "ON" : "OFF",
             binaryPending ? "*" : " ",
             stepperPercent);
    snprintf(line1, sizeof(line1), "RPM:%02u P:%04u", stepperRpm, stepperPosition);

    printPaddedLine(0, line0);
    printPaddedLine(1, line1);
}

void LcdDisplay::showPage1(bool saturationAlert, bool highAlert, bool reportingEnabled) {
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "SAT:%s HI:%s",
             saturationAlert ? "YES" : "NO ",
             highAlert ? "YES" : "NO ");
    snprintf(line1, sizeof(line1), "SER:%s", reportingEnabled ? "ON " : "OFF");

    printPaddedLine(0, line0);
    printPaddedLine(1, line1);
}

void LcdDisplay::printPaddedLine(uint8_t row, const char* text) {
    char padded[17];
    snprintf(padded, sizeof(padded), "%-16.16s", text);
    _lcd.setCursor(0, row);
    _lcd.print(padded);
}
