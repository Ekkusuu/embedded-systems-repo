#include <Arduino.h>

#include "app/ButtonLedFsmApp.h"
#include "config/AppConfig.h"
#include "io/StdioBridge.h"

static ButtonLedFsmApp gApp;

void setup() {
    StdioBridge::begin(AppConfig::SerialBaudRate);
    gApp.begin();
}

void loop() {
    gApp.tick(millis());
}
