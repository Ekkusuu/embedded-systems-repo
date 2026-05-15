#include "LcdDisplay.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "config/AppConfig.h"

namespace {

LiquidCrystal gLcd(AppConfig::LcdRsPin,
                   AppConfig::LcdEnPin,
                   AppConfig::LcdD4Pin,
                   AppConfig::LcdD5Pin,
                   AppConfig::LcdD6Pin,
                   AppConfig::LcdD7Pin);

}  // namespace

void LcdDisplay::begin() {
    gLcd.begin(16, 2);
    gLcd.clear();
    gLcd.setCursor(0, 0);
    gLcd.print("Lab9 Controller");
    gLcd.setCursor(0, 1);
    gLcd.print("Init...");
}

void LcdDisplay::showStatus(float setpointC,
                            bool sampleValid,
                            float temperatureC,
                            bool heaterOn,
                            uint8_t fanPercent) {
    gLcd.setCursor(0, 0);
    gLcd.print("SP:");
    gLcd.print(setpointC, 1);
    gLcd.print(" F:");
    if (fanPercent < 10) {
        gLcd.print('0');
    }
    gLcd.print(fanPercent);
    gLcd.print("% ");

    gLcd.setCursor(0, 1);
    gLcd.print("T:");
    if (sampleValid) {
        gLcd.print(temperatureC, 1);
        gLcd.print("C");
    } else {
        gLcd.print("---.-C");
    }
    gLcd.print(heaterOn ? " H:ON " : " H:OFF");
}
