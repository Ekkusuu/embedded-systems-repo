#pragma once
#include <stdint.h>

namespace AppConfig {

// ── Pin assignments ──────────────────────────────────────────────────────────
static constexpr uint8_t ButtonPin      = 2;   // active-LOW with INPUT_PULLUP
static constexpr uint8_t LedGreenPin    = 10;  // short-press indicator
static constexpr uint8_t LedRedPin      = 11;  // long-press indicator
static constexpr uint8_t LedYellowPin   = 12;  // statistics blink

// ── Serial ───────────────────────────────────────────────────────────────────
static constexpr uint32_t SerialBaudRate = 115200;

// ── Application thresholds ───────────────────────────────────────────────────
static constexpr uint32_t ShortPressThresholdMs = 500;  // < 500 ms => short
static constexpr uint32_t IndicatorLedOnMs      = 800;  // how long green/red stays ON

// ── Blink counts for yellow LED ──────────────────────────────────────────────
static constexpr uint8_t  BlinkCountShort       = 5;
static constexpr uint8_t  BlinkCountLong        = 10;
static constexpr uint32_t BlinkPeriodMs         = 100;  // ON / OFF interval

// ── Report interval ──────────────────────────────────────────────────────────
static constexpr uint32_t ReportIntervalMs = 10000;  // 10 s

// ── Debounce ─────────────────────────────────────────────────────────────────
static constexpr uint32_t DebounceMs = 20;

// ── Bare-metal scheduler timing (ms ticks) ───────────────────────────────────
//   Task 1 – button detect & measure: every 20 ms
static constexpr int TaskButtonRec   = 20;
static constexpr int TaskButtonOff   = 0;

//   Task 2 – statistics & yellow blink: every 20 ms, offset 5 ms
static constexpr int TaskStatsRec    = 20;
static constexpr int TaskStatsOff    = 5;

//   Task 3 – periodic STDIO report: every 10 000 ms, offset 10 ms
static constexpr int TaskReportRec   = 10000;
static constexpr int TaskReportOff   = 10;

// ── FreeRTOS task stack sizes (words) ────────────────────────────────────────
static constexpr uint16_t StackButton = 100;
static constexpr uint16_t StackStats  = 100;
static constexpr uint16_t StackReport = 150;  // needs printf stack

}  // namespace AppConfig
