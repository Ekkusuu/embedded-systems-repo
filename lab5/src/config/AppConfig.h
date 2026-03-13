#pragma once
#include <stdint.h>

namespace AppConfig {

// Pin assignments
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

// Serial
static constexpr uint32_t SerialBaudRate = 115200;

// Timing
static constexpr uint32_t AcquisitionPeriodMs = 40;  // 20-100ms requirement.
static constexpr uint32_t ReportPeriodMs = 500;
static constexpr uint32_t DhtMinReadIntervalMs = 2000;  // DHT11 datasheet min is 1000ms; Adafruit library enforces 2000ms internally.

// Pot-controlled threshold range with fixed hysteresis around the setpoint.
static constexpr float AlertSetpointMinC = 20.0f;
static constexpr float AlertSetpointMaxC = 35.0f;
static constexpr float AlertHysteresisHalfBandC = 1.0f;
static constexpr float AlertSetpointDefaultC =
	(AlertSetpointMinC + AlertSetpointMaxC) * 0.5f;
static constexpr float AlertThresholdHighC =
	AlertSetpointDefaultC + AlertHysteresisHalfBandC;
static constexpr float AlertThresholdLowC =
	AlertSetpointDefaultC - AlertHysteresisHalfBandC;

// Persistence filter for binary conditioned state.
// At 40ms acquisition this means 80ms confirmation (<100ms).
static constexpr uint8_t AlertPersistenceSamples = 2;

// NTC calibration constants (B-parameter model).
static constexpr float AdcVref = 5.0f;
static constexpr float NtcRFixedOhm = 10000.0f;
static constexpr float NtcR25Ohm = 10000.0f;
static constexpr float NtcBeta = 3950.0f;
static constexpr float NtcT0Kelvin = 298.15f;

}  // namespace AppConfig
