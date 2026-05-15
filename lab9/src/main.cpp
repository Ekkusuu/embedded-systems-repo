#include <Arduino.h>

#include "actuators/HeaterActuator.h"
#include "actuators/L293dFanActuator.h"
#include "app/Lab9ControlApp.h"
#include "config/AppConfig.h"
#include "control/OnOffHysteresisController.h"
#include "inputs/PotentiometerInput.h"
#include "io/CommandInterface.h"
#include "io/LcdDisplay.h"
#include "io/StdioBridge.h"
#include "sensors/ThermistorSensor.h"

static ThermistorSensor gTemperatureSensor(AppConfig::ThermistorPin);
static PotentiometerInput gSetpointInput(AppConfig::SetpointPotPin,
                                         AppConfig::SetpointMinC,
                                         AppConfig::SetpointMaxC);
static HeaterActuator gHeater(AppConfig::HeaterPin);
static L293dFanActuator gFan(AppConfig::FanIn1Pin,
                             AppConfig::FanIn2Pin,
                             AppConfig::FanEnablePin);
static OnOffHysteresisController gController(AppConfig::DefaultSetpointC,
                                             AppConfig::HysteresisHalfBandC);
static CommandInterface gCommandInterface;
static LcdDisplay gLcdDisplay;
static Lab9ControlApp gApp(gTemperatureSensor,
                           gSetpointInput,
                           gHeater,
                           gFan,
                           gController,
                           gCommandInterface,
                           gLcdDisplay);

void setup() {
    StdioBridge::begin(AppConfig::SerialBaudRate);
    gApp.begin();
}

void loop() {
    gApp.tick(millis());
}
