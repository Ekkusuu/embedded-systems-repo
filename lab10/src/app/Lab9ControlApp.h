#pragma once

#include <stdint.h>

#include "actuators/HeaterActuator.h"
#include "actuators/L293dFanActuator.h"
#include "control/OnOffHysteresisController.h"
#include "inputs/PotentiometerInput.h"
#include "io/CommandInterface.h"
#include "io/LcdDisplay.h"
#include "sensors/ThermistorSensor.h"

class Lab9ControlApp {
public:
    Lab9ControlApp(ThermistorSensor& sensor,
                   PotentiometerInput& setpointInput,
                   HeaterActuator& heater,
                   L293dFanActuator& fan,
                   OnOffHysteresisController& controller,
                   CommandInterface& commandInterface,
                   LcdDisplay& lcdDisplay);

    void begin();
    void tick(uint32_t nowMs);

private:
    void processCommands();
    void runSetpointTask(uint32_t nowMs);
    void runAcquisitionTask(uint32_t nowMs);
    void runControlTask(uint32_t nowMs);
    void runReportTask(uint32_t nowMs);
    void printHelp() const;
    void printStatus() const;

    ThermistorSensor& _sensor;
    PotentiometerInput& _setpointInput;
    HeaterActuator& _heater;
    L293dFanActuator& _fan;
    OnOffHysteresisController& _controller;
    CommandInterface& _commandInterface;
    LcdDisplay& _lcdDisplay;

    TemperatureSample _sample;
    uint32_t _lastSetpointMs;
    uint32_t _lastAcquisitionMs;
    uint32_t _lastControlMs;
    uint32_t _lastReportMs;
};
