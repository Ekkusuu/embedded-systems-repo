#include "TaskButtonMeasure.h"

#include <Arduino.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

#ifdef PART_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
extern SemaphoreHandle_t xButtonSemaphore;
extern SemaphoreHandle_t xStatsMutex;
#endif

// Module-scoped pointers — set once by task_button_init().
static ButtonInput*   sButton = nullptr;
static LedController* sLeds   = nullptr;

// Track when the indicator LED was turned on so we can extinguish it later.
static uint32_t indicatorOnMs   = 0;
static bool     indicatorActive = false;

void task_button_init(ButtonInput& btn, LedController& leds) {
    sButton = &btn;
    sLeds   = &leds;
    sButton->begin();
    // leds.begin() called by task_stats_init() since it owns yellow too
}

// ---------------------------------------------------------------------------
// Core logic – shared between bare-metal and FreeRTOS versions.
// ---------------------------------------------------------------------------
static void process_button(uint32_t nowMs) {
    // 1. Advance the debounce state machine.
    sButton->update(nowMs);

    // 2. Check whether the indicator LED should be switched off.
    if (indicatorActive &&
        (nowMs - indicatorOnMs) >= AppConfig::IndicatorLedOnMs) {
        sLeds->clearIndicator();
        indicatorActive = false;
    }

    // 3. Was a complete press just detected?
    if (!sButton->wasReleased()) return;

    uint32_t duration = sButton->getLastPressDurationMs();
    bool     isShort  = (duration < AppConfig::ShortPressThresholdMs);

    // 4. Light the appropriate indicator LED.
    if (isShort) {
        sLeds->indicateShortPress();
    } else {
        sLeds->indicateLongPress();
    }
    indicatorOnMs   = nowMs;
    indicatorActive = true;

    // 5. Publish result to gShared.
#ifdef PART_FREERTOS
    xSemaphoreTake(xStatsMutex, portMAX_DELAY);
#endif
    gShared.lastPressDurationMs = duration;
    gShared.newPressAvailable   = true;
#ifdef PART_FREERTOS
    xSemaphoreGive(xStatsMutex);
    // Signal Task 2 that a new press event is ready.
    xSemaphoreGive(xButtonSemaphore);
#endif
}

// ---------------------------------------------------------------------------
// Bare-metal entry point – called by the scheduler every TaskButtonRec ms.
// Uses sysTick_ms (from the ISR) for timing.
// ---------------------------------------------------------------------------
void task_button_run() {
#ifdef PART_BAREMETAL
    extern volatile uint32_t sysTick_ms;
    uint32_t now;
    noInterrupts();
    now = sysTick_ms;
    interrupts();
    process_button(now);
#endif
}

// ---------------------------------------------------------------------------
// FreeRTOS entry point – runs as a dedicated RTOS task at 20 ms recurrence.
// ---------------------------------------------------------------------------
#ifdef PART_FREERTOS
void task_button_rtos(void* pvParams) {
    (void)pvParams;

    const TickType_t offset = pdMS_TO_TICKS(AppConfig::TaskButtonOff == 0
                                            ? 1 : AppConfig::TaskButtonOff);
    const TickType_t period = pdMS_TO_TICKS(AppConfig::TaskButtonRec);

    vTaskDelay(offset);
    TickType_t xLastWake = xTaskGetTickCount();

    for (;;) {
        process_button(pdTICKS_TO_MS(xTaskGetTickCount()));
        vTaskDelayUntil(&xLastWake, period);     // precise 20 ms recurrence
    }
}
#endif
