#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace AppConfig {

static constexpr uint8_t NtcPin = A0;
static constexpr uint8_t PotPin = A1;
static constexpr uint8_t DhtPin = 8;
static constexpr uint8_t LcdRsPin = 2;
static constexpr uint8_t LcdEnPin = 3;
static constexpr uint8_t LcdD4Pin = 4;
static constexpr uint8_t LcdD5Pin = 5;
static constexpr uint8_t LcdD6Pin = 6;
static constexpr uint8_t LcdD7Pin = 7;
static constexpr uint8_t LedGreenPin = 12;
static constexpr uint8_t LedRedPin = 13;
static constexpr uint8_t BuzzerPin = 11;

static constexpr uint32_t SerialBaudRate = 115200;

static constexpr uint32_t AcquisitionPeriodMs = 40;
static constexpr uint32_t ReportPeriodMs = 500;

static constexpr float AlertSetpointMinC = 20.0f;
static constexpr float AlertSetpointMaxC = 35.0f;
static constexpr float AlertHysteresisHalfBandC = 1.0f;
static constexpr float AlertSetpointDefaultC =
    (AlertSetpointMinC + AlertSetpointMaxC) * 0.5f;
static constexpr uint8_t AlertPersistenceSamples = 2;

static constexpr float TemperatureMinC = -20.0f;
static constexpr float TemperatureMaxC = 80.0f;

static constexpr uint8_t MedianWindowSize = 3;
static constexpr uint8_t WeightedWindowSize = 4;
static constexpr uint8_t WeightedCoefficients[WeightedWindowSize] = {50, 25, 15, 10};

static constexpr float AdcVref = 5.0f;
static constexpr float NtcRFixedOhm = 10000.0f;
static constexpr float NtcR25Ohm = 10000.0f;
static constexpr float NtcBeta = 3950.0f;
static constexpr float NtcT0Kelvin = 298.15f;

}  // namespace AppConfig
