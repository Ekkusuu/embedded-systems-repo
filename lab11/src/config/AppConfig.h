#pragma once

#include <stdint.h>

namespace AppConfig {

static constexpr uint8_t LedPin = 9;
static constexpr uint8_t ButtonPin = 12;
static constexpr uint8_t LcdRsPin = 7;
static constexpr uint8_t LcdEnPin = 6;
static constexpr uint8_t LcdD4Pin = 5;
static constexpr uint8_t LcdD5Pin = 4;
static constexpr uint8_t LcdD6Pin = 3;
static constexpr uint8_t LcdD7Pin = 13;

static constexpr uint32_t SerialBaudRate = 115200;
static constexpr uint32_t DebounceDelayMs = 40;
static constexpr uint32_t ReportPeriodMs = 500;

}  // namespace AppConfig
