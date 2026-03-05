#pragma once
#include <stdint.h>

// ============================================================
// AppConfig – compile-time constants for Lab 4 (FreeRTOS)
// ============================================================

namespace AppConfig {

// ── Pin assignments ──────────────────────────────────────────
static constexpr uint8_t ButtonPin    = 2;   // active-LOW with INPUT_PULLUP
static constexpr uint8_t LedGreenPin  = 9;   // short-press indicator
static constexpr uint8_t LedRedPin    = 10;  // long-press indicator
static constexpr uint8_t LedYellowPin = 11;  // statistics blink

// ── Serial ───────────────────────────────────────────────────
static constexpr uint32_t SerialBaudRate = 115200;

// ── Application thresholds ───────────────────────────────────
static constexpr uint32_t ShortPressThresholdMs = 500; // <500 ms => short
static constexpr uint32_t IndicatorLedOnMs      = 800; // green/red stays ON for

// ── Yellow LED blink parameters ──────────────────────────────
static constexpr uint8_t  BlinkCountShort = 5;   // blinks for short press
static constexpr uint8_t  BlinkCountLong  = 10;  // blinks for long press
static constexpr uint32_t BlinkPeriodMs   = 100; // half-period (ON / OFF) [ms]

// ── Report interval ──────────────────────────────────────────
static constexpr uint32_t ReportIntervalMs = 10000; // 10 s

// ── Debounce ─────────────────────────────────────────────────
static constexpr uint32_t DebounceMs = 20;

// ── FreeRTOS Task 1 (button): 20 ms period, no offset ────────
static constexpr uint32_t TaskButtonPeriodMs = 20;
static constexpr uint32_t TaskButtonOffsetMs = 0;

// ── FreeRTOS Task 3 (report): 10 s period, 10 ms offset ──────
static constexpr uint32_t TaskReportOffsetMs = 10;

// ── FreeRTOS task stack sizes (words = 2 bytes each on AVR) ──
// Kept minimal to fit within ATmega328P 2 KB SRAM
static constexpr uint16_t StackButton = 60;
static constexpr uint16_t StackStats  = 60;
static constexpr uint16_t StackReport = 200; // needs headroom for printf + ultoa

// ── FreeRTOS task priorities ──────────────────────────────────
// Button & Stats run at equal priority (no starvation on semaphore hand-off)
static constexpr uint8_t PrioButton = 2;
static constexpr uint8_t PrioStats  = 2;
static constexpr uint8_t PrioReport = 1; // lower: runs in background

}  // namespace AppConfig
