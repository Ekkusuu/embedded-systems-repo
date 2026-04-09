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
                           int motorPercent,
                           uint8_t motorPwm,
                           bool motorForward) {
    char line0[17];
    char line1[17];

    snprintf(line0, sizeof(line0), "B:%s%s M:%4d",
             binaryState ? "ON" : "OFF",
             binaryPending ? "*" : " ",
             motorPercent);
    snprintf(line1, sizeof(line1), "PWM:%03u DIR:%s", motorPwm, motorForward ? "F" : "R");

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
