#include <Arduino.h>

#include "config/AppConfig.h"
#include "io/StdioBridge.h"
#include "io/LcdDisplay.h"
#include "sensors/NtcSensor.h"
#include "sensors/DhtSensor.h"
#include "peripherals/LedController.h"
#include "peripherals/BuzzerController.h"
#include "app/MonitorApp.h"

static NtcSensor ntc(AppConfig::NtcPin);
static DhtSensor dht(AppConfig::DhtPin);
static LedController leds(AppConfig::LedGreenPin, AppConfig::LedRedPin);
static BuzzerController buzzer(AppConfig::BuzzerPin);
static LcdDisplay lcd(AppConfig::LcdRsPin,
                      AppConfig::LcdEnPin,
                      AppConfig::LcdD4Pin,
                      AppConfig::LcdD5Pin,
                      AppConfig::LcdD6Pin,
                      AppConfig::LcdD7Pin);
static MonitorApp app(ntc, dht, leds, buzzer, lcd);

void setup() {
    StdioBridge::begin(AppConfig::SerialBaudRate);
    app.begin();
}

void loop() {
    app.tick(millis());
}
