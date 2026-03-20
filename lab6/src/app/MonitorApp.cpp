#include "MonitorApp.h"

#include <Arduino.h>
#include <math.h>
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

long random_range(long minimum, long maximumExclusive) {
    return minimum + (rand() % (maximumExclusive - minimum));
}

float random_demo_temperature(float centerC) {
    const long offsetTenthC = random_range(-35, 36);
    return centerC + static_cast<float>(offsetTenthC) * 0.1f;
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
      _ntcConditioner(AppConfig::TemperatureMinC, AppConfig::TemperatureMaxC),
      _dhtConditioner(AppConfig::TemperatureMinC, AppConfig::TemperatureMaxC),
      _ntcAlertFilter(AppConfig::AlertSetpointDefaultC + AppConfig::AlertHysteresisHalfBandC,
                      AppConfig::AlertSetpointDefaultC - AppConfig::AlertHysteresisHalfBandC,
                      AppConfig::AlertPersistenceSamples),
      _dhtAlertFilter(AppConfig::AlertSetpointDefaultC + AppConfig::AlertHysteresisHalfBandC,
                      AppConfig::AlertSetpointDefaultC - AppConfig::AlertHysteresisHalfBandC,
                      AppConfig::AlertPersistenceSamples),
      _ntcSample{},
      _dhtSample{},
      _ntcValue{},
      _dhtValue{},
      _ntcAlert{false, false, 0},
      _dhtAlert{false, false, 0},
      _potRaw(0),
      _currentSetpointC(AppConfig::AlertSetpointDefaultC),
      _currentThresholdHighC(AppConfig::AlertSetpointDefaultC + AppConfig::AlertHysteresisHalfBandC),
      _currentThresholdLowC(AppConfig::AlertSetpointDefaultC - AppConfig::AlertHysteresisHalfBandC),
      _systemAlert(false),
      _signalStable(true),
      _demoMode(false),
      _lastButtonLevel(true),
      _lastButtonChangeMs(0),
      _lastAcquisitionMs(0),
      _lastReportMs(0) {}

void MonitorApp::begin() {
    char minBuf[8];
    char maxBuf[8];

    _ntcSensor.begin();
    _dhtSensor.begin();
    _leds.begin();
    _buzzer.begin();
    _lcd.begin();
    pinMode(AppConfig::PotPin, INPUT);
    pinMode(AppConfig::DemoButtonPin, INPUT_PULLUP);
    srand(static_cast<unsigned>(analogRead(A2)));

    _ntcConditioner.reset();
    _dhtConditioner.reset();
    _ntcAlertFilter.reset(false);
    _dhtAlertFilter.reset(false);

    format_float(AppConfig::TemperatureMinC, minBuf, 5, 1);
    format_float(AppConfig::TemperatureMaxC, maxBuf, 5, 1);

    printf("Lab6 - Part2 analog conditioning monitor\n");
    printf("Same Wokwi diagram as lab5 plus blue stability LED on D10\n");
    printf("Conditioning chain: saturation[%s..%s] -> median(3) -> weighted average[50,25,15,10]\n",
           minBuf,
           maxBuf);
    printf("Alerts: hysteresis around setpoint from potentiometer, confirmation=%u samples\n",
           AppConfig::AlertPersistenceSamples);
    printf("Blue LED stability: steady=stable blinking=unstable (|raw-avg| <= %.2fC)\n",
           static_cast<double>(AppConfig::StabilityDeviationThresholdC));
    printf("Demo mode: press button on D9 to toggle random noisy data generation\n");
    printf("Periods: acquisition=%lums reporting=%lums\n",
           AppConfig::AcquisitionPeriodMs,
           AppConfig::ReportPeriodMs);
    printf("--------------------------------------------------------------\n");
}

void MonitorApp::tick(uint32_t nowMs) {
    updateDemoButton(nowMs);

    if (nowMs - _lastAcquisitionMs >= AppConfig::AcquisitionPeriodMs) {
        _lastAcquisitionMs = nowMs;
        runAcquisitionAndConditioning();
        _leds.setStability(_signalStable, nowMs);
    }

    if (nowMs - _lastReportMs >= AppConfig::ReportPeriodMs) {
        _lastReportMs = nowMs;
        runReport(nowMs);
    }
}

void MonitorApp::updateDemoButton(uint32_t nowMs) {
    const bool buttonLevel = digitalRead(AppConfig::DemoButtonPin) != LOW;

    if (buttonLevel != _lastButtonLevel) {
        if (nowMs - _lastButtonChangeMs >= AppConfig::ButtonDebounceMs) {
            _lastButtonChangeMs = nowMs;
            _lastButtonLevel = buttonLevel;

            if (!buttonLevel) {
                _demoMode = !_demoMode;
                printf("Demo mode %s\n", _demoMode ? "ON" : "OFF");
            }
        }
    }
}

void MonitorApp::runAcquisitionAndConditioning() {
    _potRaw = analogRead(AppConfig::PotPin);
    _currentSetpointC = map_pot_to_setpoint(_potRaw);
    _currentThresholdHighC = _currentSetpointC + AppConfig::AlertHysteresisHalfBandC;
    _currentThresholdLowC = _currentSetpointC - AppConfig::AlertHysteresisHalfBandC;

    _ntcAlertFilter.setThresholds(_currentThresholdHighC, _currentThresholdLowC);
    _dhtAlertFilter.setThresholds(_currentThresholdHighC, _currentThresholdLowC);

    _ntcSample = _ntcSensor.readSample();
    _dhtSample = _dhtSensor.readSample();

    if (_demoMode) {
        _ntcSample.raw = static_cast<uint16_t>(random_range(300, 701));
        _ntcSample.voltage = (static_cast<float>(_ntcSample.raw) * AppConfig::AdcVref) / 1023.0f;
        _ntcSample.temperatureC = random_demo_temperature(AppConfig::DemoBaseTemperatureC);

        _dhtSample.valid = true;
        _dhtSample.fresh = true;
        _dhtSample.temperatureC = random_demo_temperature(AppConfig::DemoBaseTemperatureC + 0.8f);

        if (random_range(0, 5) == 0) {
            _ntcSample.temperatureC += AppConfig::DemoNoiseAmplitudeC;
        }
        if (random_range(0, 5) == 0) {
            _dhtSample.temperatureC -= AppConfig::DemoNoiseAmplitudeC;
        }
    }

    _ntcValue = _ntcConditioner.update(_ntcSample.temperatureC);
    _ntcAlert = _ntcAlertFilter.update(_ntcValue.weighted);

    _buzzer.setAlert(false);
    if (_dhtSample.valid) {
        _dhtValue = _dhtConditioner.update(_dhtSample.temperatureC);
        _dhtAlert = _dhtAlertFilter.update(_dhtValue.weighted);
    }

    const bool ntcStable = fabs(_ntcValue.raw - _ntcValue.weighted) <= AppConfig::StabilityDeviationThresholdC;
    const bool dhtStable = !_dhtSample.valid ||
                           fabs(_dhtValue.raw - _dhtValue.weighted) <= AppConfig::StabilityDeviationThresholdC;
    _signalStable = ntcStable && dhtStable;

    _systemAlert = _ntcAlert.stableHigh || _dhtAlert.stableHigh;
    _leds.setAlert(_systemAlert);
    _buzzer.setAlert(_systemAlert);
}

void MonitorApp::runReport(uint32_t nowMs) const {
    char ntcRaw[8];
    char ntcSat[8];
    char ntcMed[8];
    char ntcAvg[8];
    char ntcVolt[8];
    char dhtRaw[8];
    char dhtSat[8];
    char dhtMed[8];
    char dhtAvg[8];
    char setBuf[8];
    char highBuf[8];
    char lowBuf[8];

    format_float(_ntcValue.raw, ntcRaw, 6, 2);
    format_float(_ntcValue.saturated, ntcSat, 6, 2);
    format_float(_ntcValue.median, ntcMed, 6, 2);
    format_float(_ntcValue.weighted, ntcAvg, 6, 2);
    format_float(_ntcSample.voltage, ntcVolt, 4, 2);
    format_float(_dhtValue.raw, dhtRaw, 6, 2);
    format_float(_dhtValue.saturated, dhtSat, 6, 2);
    format_float(_dhtValue.median, dhtMed, 6, 2);
    format_float(_dhtValue.weighted, dhtAvg, 6, 2);
    format_float(_currentSetpointC, setBuf, 5, 1);
    format_float(_currentThresholdHighC, highBuf, 5, 1);
    format_float(_currentThresholdLowC, lowBuf, 5, 1);

    printf("t=%lums\n", nowMs);
    printf("  NTC adc=%4u V=%s Traw=%sC sat=%s med=%s avg=%s cand=%u stable=%u\n",
           _ntcSample.raw,
           ntcVolt,
           ntcRaw,
           ntcSat,
           ntcMed,
           ntcAvg,
           _ntcAlert.candidateHigh ? 1 : 0,
           _ntcAlert.stableHigh ? 1 : 0);

    if (_dhtSample.valid) {
        printf("  DHT valid=%u fresh=%u Traw=%sC sat=%s med=%s avg=%s cand=%u stable=%u\n",
               _dhtSample.valid ? 1 : 0,
               _dhtSample.fresh ? 1 : 0,
               dhtRaw,
               dhtSat,
               dhtMed,
               dhtAvg,
               _dhtAlert.candidateHigh ? 1 : 0,
               _dhtAlert.stableHigh ? 1 : 0);
    } else {
        printf("  DHT valid=0 waiting for first sample\n");
    }

    printf("  TH set=%sC on>=%sC off<=%sC pot=%4u SYS=%u STB=%u blue=%s demo=%u\n",
           setBuf,
           highBuf,
           lowBuf,
           _potRaw,
           _systemAlert ? 1 : 0,
           _signalStable ? 1 : 0,
           _signalStable ? "ON" : "BLINK",
           _demoMode ? 1 : 0);

    _lcd.showStatus(_ntcValue.weighted,
                    _dhtValue.weighted,
                    _dhtSample.valid,
                    _systemAlert,
                    _currentThresholdHighC);
}
