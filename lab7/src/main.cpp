#include <Arduino.h>

#include "actuators/BinaryActuator.h"
#include "actuators/Stepper28BYJ48Actuator.h"
#include "app/ActuatorLabApp.h"
#include "config/AppConfig.h"
#include "io/LcdDisplay.h"
#include "io/StdioBridge.h"

static BinaryActuator gBinaryActuator(AppConfig::BinaryActuatorPin);
static Stepper28BYJ48Actuator gStepperActuator(AppConfig::Uln2003In1Pin,
                                               AppConfig::Uln2003In3Pin,
                                               AppConfig::Uln2003In2Pin,
                                               AppConfig::Uln2003In4Pin);
static LcdDisplay gLcd(AppConfig::LcdRsPin,
                       AppConfig::LcdEnPin,
                       AppConfig::LcdD4Pin,
                       AppConfig::LcdD5Pin,
                       AppConfig::LcdD6Pin,
                       AppConfig::LcdD7Pin);
static ActuatorLabApp gApp(gBinaryActuator,
                           gStepperActuator,
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
