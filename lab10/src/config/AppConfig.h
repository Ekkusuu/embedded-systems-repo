#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace AppConfig {

static constexpr uint8_t ThermistorPin = A0;
static constexpr uint8_t SetpointPotPin = A1;
static constexpr uint8_t HeaterPin = 8;
static constexpr uint8_t HeaterIndicatorPin = 12;
static constexpr uint8_t FanIn1Pin = 2;
static constexpr uint8_t FanIn2Pin = 10;
static constexpr uint8_t FanEnablePin = 11;
static constexpr uint8_t LcdRsPin = 7;
static constexpr uint8_t LcdEnPin = 6;
static constexpr uint8_t LcdD4Pin = 5;
static constexpr uint8_t LcdD5Pin = 4;
static constexpr uint8_t LcdD6Pin = 3;
static constexpr uint8_t LcdD7Pin = 13;

static constexpr uint32_t SerialBaudRate = 115200;

static constexpr uint32_t AcquisitionPeriodMs = 100;
static constexpr uint32_t SetpointPeriodMs = 100;
static constexpr uint32_t ControlPeriodMs = 250;
static constexpr uint32_t ReportPeriodMs = 500;

static constexpr float SetpointMinC = 20.0f;
static constexpr float SetpointMaxC = 80.0f;
static constexpr float DefaultSetpointC = 28.0f;
static constexpr float HysteresisHalfBandC = 1.0f;
static constexpr float FanStartDeltaC = 1.0f;
static constexpr float FanFullDeltaC = 10.0f;
static constexpr uint32_t FanRampPeriodMs = 100;
static constexpr uint8_t FanRampStepPercent = 5;

static constexpr float AdcVref = 5.0f;
static constexpr float ThermistorSeriesResistorOhm = 10000.0f;
static constexpr float ThermistorNominalOhm = 10000.0f;
static constexpr float ThermistorNominalKelvin = 298.15f;
static constexpr float ThermistorBeta = 3950.0f;

}  // namespace AppConfig
