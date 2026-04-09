#include "ActuatorLabApp.h"

#include <Arduino.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../config/AppConfig.h"

namespace {

void toLowerCaseCopy(const char* source, char* destination, size_t destinationSize) {
    if (destinationSize == 0) {
        return;
    }

    size_t i = 0;
    for (; source[i] != '\0' && i < (destinationSize - 1); ++i) {
        destination[i] = static_cast<char>(tolower(static_cast<unsigned char>(source[i])));
    }
    destination[i] = '\0';
}

}  // namespace

ActuatorLabApp::ActuatorLabApp(BinaryActuator& binaryActuator,
                               L293dDcMotorActuator& dcMotorActuator,
                               LcdDisplay& lcdDisplay,
                               Stream& primaryCommandStream,
                               Stream* secondaryCommandStream)
    : _binaryActuator(binaryActuator),
      _dcMotorActuator(dcMotorActuator),
      _lcdDisplay(lcdDisplay),
      _commandInterface(primaryCommandStream, secondaryCommandStream),
      _binaryConditioner(AppConfig::BinaryDebounceMs),
      _analogConditioner(AppConfig::AnalogPercentMin,
                         AppConfig::AnalogPercentMax,
                         AppConfig::AnalogFilterAlpha,
                         AppConfig::AnalogRampStepPercent,
                         AppConfig::AnalogHighAlertPercent) {}

void ActuatorLabApp::begin() {
    _binaryActuator.begin();
    _dcMotorActuator.begin();
    _lcdDisplay.begin();
    _commandInterface.begin();

    printf("\nLab 7 - Actuator Control\n");
    printHelp();
    printStatus();
}

void ActuatorLabApp::tick(uint32_t nowMs) {
    _dcMotorActuator.tick(nowMs);
    processCommands(nowMs);

    if ((nowMs - _lastControlMs) >= AppConfig::ControlPeriodMs) {
        _lastControlMs = nowMs;
        runControlTask(nowMs);
    }

    if ((nowMs - _lastReportMs) >= AppConfig::ReportPeriodMs) {
        _lastReportMs = nowMs;
        runReportTask(nowMs);
    }

    if ((nowMs - _lastLcdMs) >= AppConfig::LcdPagePeriodMs) {
        _lastLcdMs = nowMs;
        _lcdPage = !_lcdPage;
    }

    updateLcd(nowMs);
}

void ActuatorLabApp::processCommands(uint32_t nowMs) {
    char line[48];
    if (_commandInterface.pollLine(line, sizeof(line))) {
        handleCommand(line, nowMs);
    }
}

void ActuatorLabApp::handleCommand(const char* line, uint32_t nowMs) {
    char normalized[48];
    toLowerCaseCopy(line, normalized, sizeof(normalized));

    if (strcmp(normalized, "help") == 0) {
        printHelp();
        return;
    }

    if (strcmp(normalized, "status") == 0) {
        printStatus();
        return;
    }

    if (strcmp(normalized, "bin on") == 0) {
        _binaryConditioner.submit(true, nowMs);
        printf("CMD: binary request -> ON\n");
        return;
    }

    if (strcmp(normalized, "bin off") == 0) {
        _binaryConditioner.submit(false, nowMs);
        printf("CMD: binary request -> OFF\n");
        return;
    }

    if (strcmp(normalized, "bin toggle") == 0) {
        _binaryConditioner.submit(!_binaryConditioner.getStableState(), nowMs);
        printf("CMD: binary request -> TOGGLE\n");
        return;
    }

    int analogPercent = 0;
    if (sscanf(normalized, "ana %d", &analogPercent) == 1) {
        _analogConditioner.setTargetPercent(analogPercent);
        printf("CMD: dc motor speed request -> %d%%\n", analogPercent);
        return;
    }

    if (strcmp(normalized, "report on") == 0) {
        _reportingEnabled = true;
        printf("CMD: periodic reporting enabled\n");
        return;
    }

    if (strcmp(normalized, "report off") == 0) {
        _reportingEnabled = false;
        printf("CMD: periodic reporting disabled\n");
        return;
    }

    printf("ERR: unknown command -> %s\n", line);
    printHelp();
}

void ActuatorLabApp::runControlTask(uint32_t nowMs) {
    bool committedBinaryState = false;
    if (_binaryConditioner.tick(nowMs, &committedBinaryState)) {
        _binaryActuator.setState(committedBinaryState);
    }

    _analogConditioner.tick();
    _dcMotorActuator.setSpeedPercent(_analogConditioner.getOutputPercent());
}

void ActuatorLabApp::runReportTask(uint32_t nowMs) {
    (void)nowMs;

    if (!_reportingEnabled) {
        return;
    }

    printStatus();
}

void ActuatorLabApp::updateLcd(uint32_t nowMs) {
    (void)nowMs;

    if (!_lcdPage) {
        _lcdDisplay.showPage0(_binaryActuator.getState(),
                              _binaryConditioner.hasPendingChange(),
                              _analogConditioner.getOutputPercent(),
                              _dcMotorActuator.getSpeedPwm(),
                              _dcMotorActuator.isForward());
        return;
    }

    _lcdDisplay.showPage1(_analogConditioner.isSaturationActive(),
                          _analogConditioner.isHighAlertActive(),
                          _reportingEnabled);
}

void ActuatorLabApp::printHelp() const {
    printf("Commands:\n");
    printf("  help\n");
    printf("  status\n");
    printf("  bin on | bin off | bin toggle\n");
    printf("  ana <-100..100>  // signed dc motor speed\n");
    printf("  report on | report off\n");
}

void ActuatorLabApp::printStatus() const {
    printf("STAT bin=%s pending=%s motor_req=%d%% motor_sat=%d%% motor_out=%d%% pwm=%u dir=%s sat_alert=%s high_alert=%s report=%s\n",
           _binaryActuator.getState() ? "ON" : "OFF",
           _binaryConditioner.hasPendingChange() ? "YES" : "NO",
           _analogConditioner.getRequestedPercent(),
           _analogConditioner.getSaturatedTargetPercent(),
           _analogConditioner.getOutputPercent(),
           _dcMotorActuator.getSpeedPwm(),
           _dcMotorActuator.isForward() ? "FWD" : "REV",
           _analogConditioner.isSaturationActive() ? "YES" : "NO",
           _analogConditioner.isHighAlertActive() ? "YES" : "NO",
           _reportingEnabled ? "ON" : "OFF");
}
