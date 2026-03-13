#include "MonitorApp.h"

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "config/AppConfig.h"

namespace {

void format_float(float value, char* out, uint8_t width, uint8_t precision) {
    dtostrf(value, width, precision, out);
}

float map_pot_to_setpoint(uint16_t rawPot) {
    const float ratio = static_cast<float>(rawPot) / 1023.0f;
    return AppConfig::AlertSetpointMinC +
           ratio * (AppConfig::AlertSetpointMaxC - AppConfig::AlertSetpointMinC);
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
    _potRaw(0),
    _currentSetpointC(AppConfig::AlertSetpointDefaultC),
    _currentThresholdHighC(AppConfig::AlertThresholdHighC),
    _currentThresholdLowC(AppConfig::AlertThresholdLowC),
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
    pinMode(AppConfig::PotPin, INPUT);

    printf("Lab5 - Dual sensor threshold monitor (NO FreeRTOS)\n");
    printf("Sensors: Thermistor(A0 analog), DHT11(D8 digital)\n");
        format_float(_currentThresholdHighC, highBuf, 4, 1);
        format_float(_currentThresholdLowC, lowBuf, 4, 1);

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
    (void)nowMs;
    _potRaw = analogRead(AppConfig::PotPin);
    _currentSetpointC = map_pot_to_setpoint(_potRaw);
    _currentThresholdHighC = _currentSetpointC + AppConfig::AlertHysteresisHalfBandC;
    _currentThresholdLowC = _currentSetpointC - AppConfig::AlertHysteresisHalfBandC;
    _ntcConditioner.setThresholds(_currentThresholdHighC, _currentThresholdLowC);
    _dhtConditioner.setThresholds(_currentThresholdHighC, _currentThresholdLowC);

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
    char spTemp[8];
    char thHigh[8];
    char thLow[8];

    format_float(_ntcSample.temperatureC, ntcTemp, 7, 2);
    format_float(_ntcSample.voltage, ntcVoltage, 5, 2);
    format_float(_dhtSample.temperatureC, dhtTemp, 7, 2);
    format_float(_currentSetpointC, spTemp, 4, 1);
    format_float(_currentThresholdHighC, thHigh, 4, 1);
    format_float(_currentThresholdLowC, thLow, 4, 1);

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

        printf("  POT raw=%4u set=%sC TH[ON>=%sC OFF<=%sC]\n",
            _potRaw,
            spTemp,
            thHigh,
            thLow);

    _lcd.showStatus(_ntcSample.temperatureC,
                    _dhtSample.temperatureC,
                    _dhtSample.valid,
                    _systemAlert,
                  _currentThresholdHighC,
                  _currentThresholdLowC);
}
