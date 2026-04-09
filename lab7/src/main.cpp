#include <Arduino.h>

#include "actuators/BinaryActuator.h"
#include "actuators/L293dDcMotorActuator.h"
#include "app/ActuatorLabApp.h"
#include "config/AppConfig.h"
#include "io/LcdDisplay.h"
#include "io/StdioBridge.h"

static BinaryActuator gBinaryActuator(AppConfig::BinaryActuatorPin);
static L293dDcMotorActuator gDcMotorActuator(AppConfig::L293dIn1Pin,
                                             AppConfig::L293dIn2Pin,
                                             AppConfig::L293dEnablePin);
static LcdDisplay gLcd(AppConfig::LcdRsPin,
                       AppConfig::LcdEnPin,
                       AppConfig::LcdD4Pin,
                       AppConfig::LcdD5Pin,
                       AppConfig::LcdD6Pin,
                       AppConfig::LcdD7Pin);
static ActuatorLabApp gApp(gBinaryActuator,
                           gDcMotorActuator,
                           gLcd,
                           Serial,
                           nullptr);

void setup() {
    StdioBridge::begin(AppConfig::SerialBaudRate);
    gApp.begin();
}

void loop() {
    gApp.tick(millis());
}
