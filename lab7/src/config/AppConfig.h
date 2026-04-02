#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace AppConfig {

static constexpr uint32_t SerialBaudRate = 115200;
static constexpr uint32_t TerminalBaudRate = 9600;

static constexpr uint8_t LcdRsPin = 7;
static constexpr uint8_t LcdEnPin = 6;
static constexpr uint8_t LcdD4Pin = 5;
static constexpr uint8_t LcdD5Pin = 4;
static constexpr uint8_t LcdD6Pin = 3;
static constexpr uint8_t LcdD7Pin = 13;

static constexpr uint8_t Uln2003In1Pin = 8;
static constexpr uint8_t Uln2003In2Pin = 9;
static constexpr uint8_t Uln2003In3Pin = 10;
static constexpr uint8_t Uln2003In4Pin = 11;
static constexpr uint8_t BinaryActuatorPin = 12;

static constexpr uint32_t ControlPeriodMs = 20;
static constexpr uint32_t ReportPeriodMs = 500;
static constexpr uint32_t LcdPagePeriodMs = 2000;
static constexpr uint32_t BinaryDebounceMs = 60;

static constexpr int AnalogPercentMin = 0;
static constexpr int AnalogPercentMax = 100;
static constexpr float AnalogFilterAlpha = 0.25f;
static constexpr float AnalogRampStepPercent = 4.0f;
static constexpr uint8_t AnalogHighAlertPercent = 85;

}  // namespace AppConfig
