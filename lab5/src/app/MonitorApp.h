#pragma once

#include <stdint.h>

#include "peripherals/LedController.h"
#include "peripherals/BuzzerController.h"
#include "sensors/NtcSensor.h"
#include "sensors/DhtSensor.h"
#include "signal/BinaryThresholdFilter.h"
#include "io/LcdDisplay.h"

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
    void runAcquisitionAndConditioning(uint32_t nowMs);
    void runReport(uint32_t nowMs) const;

    NtcSensor& _ntcSensor;
    DhtSensor& _dhtSensor;
    LedController& _leds;
    BuzzerController& _buzzer;
    LcdDisplay& _lcd;

    BinaryThresholdFilter _ntcConditioner;
    BinaryThresholdFilter _dhtConditioner;

    NtcSample _ntcSample;
    DhtSample _dhtSample;

    ThresholdState _ntcState;
    ThresholdState _dhtState;

    bool _systemAlert;
    uint32_t _lastAcquisitionMs;
    uint32_t _lastReportMs;
};
