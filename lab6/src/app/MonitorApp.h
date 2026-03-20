#pragma once

#include <stdint.h>

#include "io/LcdDisplay.h"
#include "peripherals/BuzzerController.h"
#include "peripherals/LedController.h"
#include "sensors/DhtSensor.h"
#include "sensors/NtcSensor.h"
#include "signal/AnalogConditioner.h"
#include "signal/BinaryThresholdFilter.h"

class MonitorApp {
public:
    MonitorApp(NtcSensor& ntcSensor,
               DhtSensor& dhtSensor,
               LedController& leds,
               BuzzerController& buzzer,
               LcdDisplay& lcd);

    void begin();
    void tick(uint32_t nowMs);

private:
    void runAcquisitionAndConditioning();
    void runReport(uint32_t nowMs) const;

    NtcSensor& _ntcSensor;
    DhtSensor& _dhtSensor;
    LedController& _leds;
    BuzzerController& _buzzer;
    LcdDisplay& _lcd;

    AnalogConditioner _ntcConditioner;
    AnalogConditioner _dhtConditioner;
    BinaryThresholdFilter _ntcAlertFilter;
    BinaryThresholdFilter _dhtAlertFilter;

    NtcSample _ntcSample;
    DhtSample _dhtSample;
    AnalogConditionedValue _ntcValue;
    AnalogConditionedValue _dhtValue;
    ThresholdState _ntcAlert;
    ThresholdState _dhtAlert;

    uint16_t _potRaw;
    float _currentSetpointC;
    float _currentThresholdHighC;
    float _currentThresholdLowC;
    bool _systemAlert;
    uint32_t _lastAcquisitionMs;
    uint32_t _lastReportMs;
};
