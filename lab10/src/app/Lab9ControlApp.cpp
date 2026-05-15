#include "Lab9ControlApp.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config/AppConfig.h"

namespace {

void format_float(float value, char* out, uint8_t width, uint8_t precision) {
    dtostrf(value, width, precision, out);
}

}  // namespace

Lab9ControlApp::Lab9ControlApp(ThermistorSensor& sensor,
                               PotentiometerInput& setpointInput,
                               HeaterActuator& heater,
                               L293dFanActuator& fan,
                               OnOffHysteresisController& controller,
                               CommandInterface& commandInterface,
                               LcdDisplay& lcdDisplay)
    : _sensor(sensor),
      _setpointInput(setpointInput),
      _heater(heater),
      _fan(fan),
      _controller(controller),
      _commandInterface(commandInterface),
      _lcdDisplay(lcdDisplay),
      _sample{},
      _lastSetpointMs(0),
      _lastAcquisitionMs(0),
      _lastControlMs(0),
      _lastReportMs(0) {}

void Lab9ControlApp::begin() {
    _sensor.begin();
    _setpointInput.begin();
    _heater.begin();
    _fan.begin();
    _commandInterface.begin();
    _lcdDisplay.begin();
    _controller.setSetpoint(AppConfig::DefaultSetpointC);

    printf("Lab9 - Thermistor heater + fan control\n");
    printf("Commands: help | status\n");
    printf("Plotter CSV: setpoint,temperature,heater,fan\n");
}

void Lab9ControlApp::tick(uint32_t nowMs) {
    processCommands();

    if (nowMs - _lastSetpointMs >= AppConfig::SetpointPeriodMs) {
        _lastSetpointMs = nowMs;
        runSetpointTask(nowMs);
    }

    if (nowMs - _lastAcquisitionMs >= AppConfig::AcquisitionPeriodMs) {
        _lastAcquisitionMs = nowMs;
        runAcquisitionTask(nowMs);
    }

    if (nowMs - _lastControlMs >= AppConfig::ControlPeriodMs) {
        _lastControlMs = nowMs;
        runControlTask(nowMs);
    }

    if (nowMs - _lastReportMs >= AppConfig::ReportPeriodMs) {
        _lastReportMs = nowMs;
        runReportTask(nowMs);
    }

    _fan.tick(nowMs);
    _lcdDisplay.showStatus(
        _controller.setpoint(), _sample.valid, _sample.celsius, _heater.state(), _fan.speedPercent());
}

void Lab9ControlApp::processCommands() {
    char line[32];
    if (!_commandInterface.pollLine(line, sizeof(line))) {
        return;
    }

    if (strcmp(line, "help") == 0) {
        printHelp();
        return;
    }

    if (strcmp(line, "status") == 0) {
        printStatus();
        return;
    }

    printf("ERR unknown command\n");
    printHelp();
}

void Lab9ControlApp::runSetpointTask(uint32_t nowMs) {
    (void)nowMs;
    _controller.setSetpoint(_setpointInput.readMappedValue());
}

void Lab9ControlApp::runAcquisitionTask(uint32_t nowMs) {
    (void)nowMs;
    _sample = _sensor.readTemperature();
}

void Lab9ControlApp::runControlTask(uint32_t nowMs) {
    (void)nowMs;

    if (!_sample.valid) {
        _heater.setState(false);
        _fan.setTargetPercent(0);
        return;
    }

    _heater.setState(_controller.update(_sample.celsius));

    uint8_t fanTargetPercent = 0;
    const float fanStartC = _controller.setpoint() + AppConfig::FanStartDeltaC;
    const float fanFullC = _controller.setpoint() + AppConfig::FanFullDeltaC;

    if (_sample.celsius > fanStartC) {
        if (_sample.celsius >= fanFullC) {
            fanTargetPercent = 100;
        } else {
            const float ratio = (_sample.celsius - fanStartC) / (fanFullC - fanStartC);
            fanTargetPercent = static_cast<uint8_t>(ratio * 100.0f);
            if (fanTargetPercent < AppConfig::FanRampStepPercent) {
                fanTargetPercent = AppConfig::FanRampStepPercent;
            }
        }
    }

    _fan.setTargetPercent(fanTargetPercent);
}

void Lab9ControlApp::runReportTask(uint32_t nowMs) {
    (void)nowMs;

    if (!_sample.valid) {
        printf("nan,nan,0,0\n");
        return;
    }

    char setpointBuf[8];
    char tempBuf[8];

    format_float(_controller.setpoint(), setpointBuf, 5, 1);
    format_float(_sample.celsius, tempBuf, 5, 1);
    printf("%s,%s,%u,%u\n",
           setpointBuf,
           tempBuf,
           _heater.state() ? 1 : 0,
           _fan.speedPercent());
}

void Lab9ControlApp::printHelp() const {
    printf("help          - show commands\n");
    printf("status        - print current state\n");
    printf("setpoint is controlled by potentiometer on A1\n");
}

void Lab9ControlApp::printStatus() const {
    if (!_sample.valid) {
        char setpointBuf[8];
        format_float(_controller.setpoint(), setpointBuf, 4, 1);
        printf("STAT sp=%s temp=nan heat=%u fan=%u%%\n",
               setpointBuf,
               _heater.state() ? 1 : 0,
               _fan.speedPercent());
        return;
    }

    char setpointBuf[8];
    char tempBuf[8];
    char onBuf[8];
    char offBuf[8];

    format_float(_controller.setpoint(), setpointBuf, 4, 1);
    format_float(_sample.celsius, tempBuf, 5, 2);
    format_float(_controller.thresholdOn(), onBuf, 4, 1);
    format_float(_controller.thresholdOff(), offBuf, 4, 1);

    printf("STAT sp=%s temp=%s on<=%s off>=%s heat=%u fan=%u%% raw=%u\n",
           setpointBuf,
           tempBuf,
           onBuf,
           offBuf,
           _heater.state() ? 1 : 0,
           _fan.speedPercent(),
           _sample.raw);
}
