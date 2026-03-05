// ============================================================
// TaskButtonMeasure.cpp – Task 1 (FreeRTOS)
//
// Runs every 20 ms (vTaskDelayUntil).
// Detected presses are published to gShared and signalled to
// Task 2 through xButtonSemaphore.
// gShared is always accessed under xStatsMutex.
// ============================================================
#include "TaskButtonMeasure.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

// Sync handles – defined in main.cpp
extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xStatsMutex;

// Module-scoped hardware references (set once in task_button_init)
static ButtonInput*   sButton = nullptr;
static LedController* sLeds   = nullptr;

// Non-blocking indicator LED timeout tracking
static uint32_t indicatorOnMs   = 0;
static bool     indicatorActive = false;

// ── Initialisation ────────────────────────────────────────────
void task_button_init(ButtonInput& btn, LedController& leds) {
    sButton = &btn;
    sLeds   = &leds;
    sButton->begin();
    // LedController::begin() is called by task_stats_init() which owns
    // all three LED pins.
}

// ── FreeRTOS task ─────────────────────────────────────────────
void task_button_rtos(void* pvParams) {
    (void)pvParams;

    const TickType_t period = pdMS_TO_TICKS(AppConfig::TaskButtonPeriodMs);

    // Apply initial offset (0 ms => start immediately)
    if (AppConfig::TaskButtonOffsetMs > 0) {
        vTaskDelay(pdMS_TO_TICKS(AppConfig::TaskButtonOffsetMs));
    }

    TickType_t xLastWake = xTaskGetTickCount();

    for (;;) {
        uint32_t now = millis(); // ms timestamp for FSM and LED timeout

        // 1. Advance the debounce state machine (non-blocking)
        sButton->update(now);

        // 2. Turn off indicator LED after IndicatorLedOnMs without blocking
        if (indicatorActive &&
            (now - indicatorOnMs) >= AppConfig::IndicatorLedOnMs) {
            sLeds->clearIndicator();
            indicatorActive = false;
        }

        // 3. Was a complete press/release cycle confirmed this tick?
        if (sButton->wasReleased()) {
            uint32_t duration = sButton->getLastPressDurationMs();
            bool     isShort  = (duration < AppConfig::ShortPressThresholdMs);

            // Visual feedback: green for short, red for long
            if (isShort) {
                sLeds->indicateShortPress();
            } else {
                sLeds->indicateLongPress();
            }
            indicatorOnMs   = now;
            indicatorActive = true;

            // Publish press data – mutex guards the write so Task 3 cannot
            // read a partially updated struct while Task 2 is also writing
            xSemaphoreTake(xStatsMutex, portMAX_DELAY);
            gShared.lastPressDurationMs = duration;
            gShared.newPressAvailable   = true;
            xSemaphoreGive(xStatsMutex);

            // Signal Task 2 that a new event is ready (binary semaphore)
            xSemaphoreGive(xButtonSemaphore);
        }

        // 4. Sleep until the next 20 ms period (accounts for execution time)
        vTaskDelayUntil(&xLastWake, period);
    }
}
