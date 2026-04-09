#pragma once

#include <stdint.h>

#include <Stream.h>

#include "../actuators/BinaryActuator.h"
#include "../actuators/L293dDcMotorActuator.h"
#include "../io/CommandInterface.h"
#include "../io/LcdDisplay.h"
#include "../signal/AnalogCommandConditioner.h"
#include "../signal/BinaryCommandConditioner.h"

class ActuatorLabApp {
public:
    ActuatorLabApp(BinaryActuator& binaryActuator,
                   L293dDcMotorActuator& dcMotorActuator,
                   LcdDisplay& lcdDisplay,
                   Stream& primaryCommandStream,
                   Stream* secondaryCommandStream);

    void begin();
    void tick(uint32_t nowMs);

private:
    void processCommands(uint32_t nowMs);
    void handleCommand(const char* line, uint32_t nowMs);
    void runControlTask(uint32_t nowMs);
    void runReportTask(uint32_t nowMs);
    void updateLcd(uint32_t nowMs);
    void printHelp() const;
    void printStatus() const;

    BinaryActuator& _binaryActuator;
    L293dDcMotorActuator& _dcMotorActuator;
    LcdDisplay& _lcdDisplay;
    CommandInterface _commandInterface;
    BinaryCommandConditioner _binaryConditioner;
    AnalogCommandConditioner _analogConditioner;

    bool _reportingEnabled = false;
    uint32_t _lastControlMs = 0;
    uint32_t _lastReportMs = 0;
    uint32_t _lastLcdMs = 0;
    bool _lcdPage = false;
};
