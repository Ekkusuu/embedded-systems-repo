#include "TaskStatistics.h"

#include <Arduino.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

#ifdef PART_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xStatsMutex;
#endif

static LedController* sLeds = nullptr;

// ── Non-blocking yellow blink state machine (bare-metal only) ────────────────
#ifdef PART_BAREMETAL
static uint8_t  blinkTogglesLeft  = 0;   // each toggle = one half-period
static bool     blinkLedState     = false;
static uint32_t blinkLastToggleMs = 0;
#endif

void task_stats_init(LedController& leds) {
    sLeds = &leds;
    sLeds->begin();
}

// ---------------------------------------------------------------------------
// Update accumulated statistics – called after consuming gShared.
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// Bare-metal entry point – called by the scheduler every TaskStatsRec ms.
// ---------------------------------------------------------------------------
void task_stats_run() {
#ifdef PART_BAREMETAL
    extern volatile uint32_t sysTick_ms;
    uint32_t now;
    noInterrupts();
    now = sysTick_ms;
    interrupts();

    // 1. Handle new press event if available.
    if (gShared.newPressAvailable) {
        gShared.newPressAvailable = false;

        uint32_t duration = gShared.lastPressDurationMs;
        bool     isShort  = (duration < AppConfig::ShortPressThresholdMs);

        accumulate(duration, isShort);

        // Start blink sequence: each blink = 2 toggles (on + off).
        uint8_t blinks       = isShort ? AppConfig::BlinkCountShort
                                       : AppConfig::BlinkCountLong;
        blinkTogglesLeft  = blinks * 2;
        blinkLedState     = false;
        blinkLastToggleMs = now;
    }

    // 2. Advance the non-blocking blink state machine.
    if (blinkTogglesLeft > 0) {
        if ((now - blinkLastToggleMs) >= AppConfig::BlinkPeriodMs) {
            blinkLedState = !blinkLedState;
            sLeds->setYellow(blinkLedState);
            blinkLastToggleMs = now;
            blinkTogglesLeft--;
        }
    }
#endif
}

// ---------------------------------------------------------------------------
// FreeRTOS entry point – waits on the binary semaphore given by Task 1.
// Yellow LED blinking is done with vTaskDelay (fully blocking is fine here).
// ---------------------------------------------------------------------------
#ifdef PART_FREERTOS
void task_stats_rtos(void* pvParams) {
    (void)pvParams;

    for (;;) {
        // Block until Task 1 signals a new press event.
        if (xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE) {

            // Read and clear the shared event (mutex-protected).
            xSemaphoreTake(xStatsMutex, portMAX_DELAY);
            uint32_t duration = gShared.lastPressDurationMs;
            gShared.newPressAvailable = false;
            bool isShort = (duration < AppConfig::ShortPressThresholdMs);
            accumulate(duration, isShort);
            xSemaphoreGive(xStatsMutex);

            // Blink yellow LED – blocking inside this RTOS task is fine.
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
}
#endif
