#include "MonitorApp.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "config/AppConfig.h"

namespace {

void format_float(float value, char* out, uint8_t width, uint8_t precision) {
    dtostrf(value, width, precision, out);
}

}  // namespace

MonitorApp::MonitorApp(NtcSensor& ntcSensor,
                                             DhtSensor& dhtSensor,
                                             LedController& leds,
                                             BuzzerController& buzzer,
                                             LcdDisplay& lcd)
    : _ntcSensor(ntcSensor),
      _dhtSensor(dhtSensor),
      _leds(leds),
      _buzzer(buzzer),
            _lcd(lcd),
      _ntcConditioner(AppConfig::AlertThresholdHighC,
                      AppConfig::AlertThresholdLowC,
                      AppConfig::AlertPersistenceSamples),
      _dhtConditioner(AppConfig::AlertThresholdHighC,
                      AppConfig::AlertThresholdLowC,
                      AppConfig::AlertPersistenceSamples),
      _ntcSample{},
      _dhtSample{},
      _ntcState{false, false, 0},
      _dhtState{false, false, 0},
      _systemAlert(false),
      _lastAcquisitionMs(0),
      _lastReportMs(0) {}

void MonitorApp::begin() {
    char highBuf[8];
    char lowBuf[8];

    _ntcSensor.begin();
    _dhtSensor.begin();
    _leds.begin();
    _buzzer.begin();
    _lcd.begin();

    _ntcConditioner.reset(false);
    _dhtConditioner.reset(false);

    printf("Lab5 - Dual sensor threshold monitor (NO FreeRTOS)\n");
    printf("Sensors: NTC(A0 analog), DHT22(D8 digital)\n");
        format_float(AppConfig::AlertThresholdHighC, highBuf, 4, 1);
        format_float(AppConfig::AlertThresholdLowC, lowBuf, 4, 1);

        printf("Conditioning: hysteresis [ON>=%sC OFF<=%sC], persistence=%u samples\n",
            highBuf,
            lowBuf,
           AppConfig::AlertPersistenceSamples);
    printf("Periods: acquisition=%lums, reporting=%lums, dht_min=%lums\n",
           AppConfig::AcquisitionPeriodMs,
           AppConfig::ReportPeriodMs,
           AppConfig::DhtMinReadIntervalMs);
    printf("--------------------------------------------------------------\n");
}

void MonitorApp::tick(uint32_t nowMs) {
    if (nowMs - _lastAcquisitionMs >= AppConfig::AcquisitionPeriodMs) {
        _lastAcquisitionMs = nowMs;
        runAcquisitionAndConditioning(nowMs);
    }

    if (nowMs - _lastReportMs >= AppConfig::ReportPeriodMs) {
        _lastReportMs = nowMs;
        runReport(nowMs);
    }
}

void MonitorApp::runAcquisitionAndConditioning(uint32_t nowMs) {
    _ntcSample = _ntcSensor.readSample();

    // Silence buzzer during DHT read — tone() ISR can interfere with
    // the timing-critical DHT22 protocol in simulation.
    _buzzer.setAlert(false);
    _dhtSample = _dhtSensor.readSample();

    _ntcState = _ntcConditioner.update(_ntcSample.temperatureC);

    if (_dhtSample.valid) {
        _dhtState = _dhtConditioner.update(_dhtSample.temperatureC);
    }

    _systemAlert = _ntcState.stableHigh || _dhtState.stableHigh;
    _leds.setAlert(_systemAlert);
    _buzzer.setAlert(_systemAlert);
}

void MonitorApp::runReport(uint32_t nowMs) const {
    char ntcTemp[12];
    char ntcVoltage[10];
    char dhtTemp[12];

    format_float(_ntcSample.temperatureC, ntcTemp, 7, 2);
    format_float(_ntcSample.voltage, ntcVoltage, 5, 2);
    format_float(_dhtSample.temperatureC, dhtTemp, 7, 2);

    printf("t=%lums\n", nowMs);
    printf("  NTC raw=%4u V=%s T=%sC cand=%u stable=%u db=%u\n",
           _ntcSample.raw,
           ntcVoltage,
           ntcTemp,
           _ntcState.candidateHigh ? 1 : 0,
           _ntcState.stableHigh ? 1 : 0,
           _ntcState.pendingCounter);

    printf("  DHT valid=%u fresh=%u T=%sC cand=%u stable=%u db=%u\n",
           _dhtSample.valid ? 1 : 0,
           _dhtSample.fresh ? 1 : 0,
           dhtTemp,
           _dhtState.candidateHigh ? 1 : 0,
           _dhtState.stableHigh ? 1 : 0,
           _dhtState.pendingCounter);

    printf("  SYS alert=%u LED_G=%u LED_R=%u\n",
           _systemAlert ? 1 : 0,
           _systemAlert ? 0 : 1,
           _systemAlert ? 1 : 0);

    _lcd.showStatus(_ntcSample.temperatureC,
                    _dhtSample.temperatureC,
                    _dhtSample.valid,
                    _systemAlert);
}
