#include <Arduino.h>

#include "app/AccessControlApp.h"
#include "config/AppConfig.h"
#include "peripherals/KeypadInput.h"
#include "peripherals/LcdDisplay.h"
#include "peripherals/LedController.h"

LcdDisplay lcdDisplay(AppConfig::LcdRsPin, AppConfig::LcdEnablePin,
                      AppConfig::LcdD4Pin, AppConfig::LcdD5Pin,
                      AppConfig::LcdD6Pin, AppConfig::LcdD7Pin,
                      AppConfig::LcdColumns, AppConfig::LcdRows);

KeypadInput keypadInput((char *)AppConfig::KeyLayout, AppConfig::KeypadRows,
                        AppConfig::KeypadColumns, AppConfig::KeypadRowCount,
                        AppConfig::KeypadColumnCount);

LedController ledController(AppConfig::GreenLedPin, AppConfig::RedLedPin);

AccessControlApp accessControlApp(lcdDisplay, keypadInput, ledController,
                                  AppConfig::ValidCode,
                                  AppConfig::CodeLength);

void setup() { accessControlApp.begin(); }

void loop() { accessControlApp.loop(); }
