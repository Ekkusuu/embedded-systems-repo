// ============================================================
// TaskStatistics.cpp – Task 2 (FreeRTOS)
//
// Event-driven: blocks on xButtonSemaphore given by Task 1.
// Accumulates statistics in gShared under xStatsMutex.
// Blinks the yellow LED with vTaskDelay (blocking allowed here).
// ============================================================
#include "TaskStatistics.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

// Sync handles – defined in main.cpp
extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xStatsMutex;

static LedController* sLeds = nullptr;

// ── Initialisation ────────────────────────────────────────────
void task_stats_init(LedController& leds) {
    sLeds = &leds;
    sLeds->begin(); // initialises all three LED pins
}

// ── Statistics accumulation (called under mutex) ──────────────
static void accumulate(uint32_t duration, bool isShort) {
    gShared.totalPresses++;
    if (isShort) {
        gShared.shortPresses++;
        gShared.shortDurationSumMs += duration;
    } else {
        gShared.longPresses++;
        gShared.longDurationSumMs += duration;
    }
}

// ── FreeRTOS task ─────────────────────────────────────────────
void task_stats_rtos(void* pvParams) {
    (void)pvParams;

    for (;;) {
        // Block indefinitely until Task 1 signals a new press event.
        // This releases the CPU until the event arrives (no busy-waiting).
        xSemaphoreTake(xButtonSemaphore, portMAX_DELAY);

        // Read the event data and update statistics – mutex ensures Task 3
        // cannot read partially updated results during the same window.
        xSemaphoreTake(xStatsMutex, portMAX_DELAY);
        uint32_t duration = gShared.lastPressDurationMs;
        gShared.newPressAvailable = false;
        bool isShort = (duration < AppConfig::ShortPressThresholdMs);
        accumulate(duration, isShort);
        xSemaphoreGive(xStatsMutex);

        // Blink yellow LED: 5 blinks for short press, 10 for long press.
        // vTaskDelay yields the CPU while waiting – safe in an event-driven task.
        uint8_t blinks = isShort ? AppConfig::BlinkCountShort
                                 : AppConfig::BlinkCountLong;
        for (uint8_t i = 0; i < blinks; i++) {
            sLeds->setYellow(true);
            vTaskDelay(pdMS_TO_TICKS(AppConfig::BlinkPeriodMs));
            sLeds->setYellow(false);
            vTaskDelay(pdMS_TO_TICKS(AppConfig::BlinkPeriodMs));
        }
    }
}
