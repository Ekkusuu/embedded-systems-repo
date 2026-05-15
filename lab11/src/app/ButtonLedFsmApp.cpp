#include "ButtonLedFsmApp.h"

#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>

#include "config/AppConfig.h"

namespace {

LiquidCrystal gLcd(AppConfig::LcdRsPin,
                   AppConfig::LcdEnPin,
                   AppConfig::LcdD4Pin,
                   AppConfig::LcdD5Pin,
                   AppConfig::LcdD6Pin,
                   AppConfig::LcdD7Pin);

int lcd_putchar(char c, FILE* stream) {
    (void)stream;
    gLcd.write(c);
    return 0;
}

FILE gLcdOut;

}  // namespace

void ButtonLedFsmApp::begin() {
    pinMode(AppConfig::LedPin, OUTPUT);
    pinMode(AppConfig::ButtonPin, INPUT_PULLUP);
    gLcd.begin(16, 2);
    fdev_setup_stream(&gLcdOut, lcd_putchar, nullptr, _FDEV_SETUP_WRITE);

    _state = LedState::Off;
    _lastReading = digitalRead(AppConfig::ButtonPin) == LOW;
    _debouncedPressed = _lastReading;
    _lastDebounceMs = millis();
    _lastReportMs = 0;

    applyOutput();
    updateLcd();
    printf("Lab11 Part 1 - Button LED finite-state machine\n");
    printf("Pins: LED=D%u BUTTON=D%u (INPUT_PULLUP)\n", AppConfig::LedPin, AppConfig::ButtonPin);
    printf("Button wiring: D%u -> button -> GND\n", AppConfig::ButtonPin);
    reportState(millis(), true);
}

void ButtonLedFsmApp::tick(uint32_t nowMs) {
    pollButton(nowMs);
    reportState(nowMs, false);
}

void ButtonLedFsmApp::pollButton(uint32_t nowMs) {
    const bool reading = digitalRead(AppConfig::ButtonPin) == LOW;

    if (reading != _lastReading) {
        _lastReading = reading;
        _lastDebounceMs = nowMs;
    }

    if (nowMs - _lastDebounceMs < AppConfig::DebounceDelayMs) {
        return;
    }

    if (_debouncedPressed != reading) {
        _debouncedPressed = reading;
        if (_debouncedPressed) {
            toggleState();
            applyOutput();
            updateLcd();
            reportState(nowMs, true);
        }
    }
}

void ButtonLedFsmApp::toggleState() {
    _state = (_state == LedState::Off) ? LedState::On : LedState::Off;
}

void ButtonLedFsmApp::applyOutput() const {
    digitalWrite(AppConfig::LedPin, _state == LedState::On ? HIGH : LOW);
}

void ButtonLedFsmApp::updateLcd() const {
    gLcd.setCursor(0, 0);
    fprintf(&gLcdOut, "Lab11 FSM       ");
    gLcd.setCursor(0, 1);
    fprintf(&gLcdOut, "LED: %-3s        ", stateName());
}

void ButtonLedFsmApp::reportState(uint32_t nowMs, bool forced) {
    if (!forced && nowMs - _lastReportMs < AppConfig::ReportPeriodMs) {
        return;
    }

    _lastReportMs = nowMs;
    printf("t=%lu state=%s led=%u button=%u\n",
           nowMs,
           stateName(),
           _state == LedState::On ? 1 : 0,
           _debouncedPressed ? 1 : 0);
}

const char* ButtonLedFsmApp::stateName() const {
    return (_state == LedState::On) ? "ON" : "OFF";
}
