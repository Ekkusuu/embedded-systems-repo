#include "TaskReport.h"

#include <Arduino.h>
#include <stdio.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

#ifdef PART_FREERTOS
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
extern SemaphoreHandle_t xStatsMutex;
#endif

void task_report_init() {
    // Serial already started in main; nothing to initialise here.
}

// ---------------------------------------------------------------------------
// Print a formatted statistics report and reset the counters.
// ---------------------------------------------------------------------------
static void print_report() {
    // Snapshot values before reset.
    uint32_t total  = gShared.totalPresses;
    uint32_t shorts = gShared.shortPresses;
    uint32_t longs  = gShared.longPresses;
    uint32_t avgShort = (shorts > 0) ? (gShared.shortDurationSumMs / shorts) : 0;
    uint32_t avgLong  = (longs  > 0) ? (gShared.longDurationSumMs  / longs)  : 0;

    Serial.println(F("========================================"));
    Serial.println(F("  PERIODIC STATISTICS REPORT (10 s)"));
    Serial.println(F("========================================"));
    Serial.print(F("  Total presses     : ")); Serial.println(total);
    Serial.print(F("  Short presses     : ")); Serial.println(shorts);
    Serial.print(F("  Long  presses     : ")); Serial.println(longs);
    Serial.print(F("  Avg short dur [ms]: ")); Serial.println(avgShort);
    Serial.print(F("  Avg long  dur [ms]: ")); Serial.println(avgLong);
    Serial.println(F("  (Counters reset)"));
    Serial.println(F("========================================"));

    // Reset statistics.
    gShared.totalPresses      = 0;
    gShared.shortPresses      = 0;
    gShared.longPresses       = 0;
    gShared.shortDurationSumMs = 0;
    gShared.longDurationSumMs  = 0;
}

// ---------------------------------------------------------------------------
// Bare-metal entry point – called by the scheduler every TaskReportRec ms.
// ---------------------------------------------------------------------------
void task_report_run() {
#ifdef PART_BAREMETAL
    print_report();
#endif
}

// ---------------------------------------------------------------------------
// FreeRTOS entry point – uses vTaskDelayUntil for precise 10 s recurrence.
// ---------------------------------------------------------------------------
#ifdef PART_FREERTOS
void task_report_rtos(void* pvParams) {
    (void)pvParams;

    const TickType_t offset = pdMS_TO_TICKS(AppConfig::TaskReportOff);
    const TickType_t period = pdMS_TO_TICKS(AppConfig::ReportIntervalMs);

    vTaskDelay(offset);
    TickType_t xLastWake = xTaskGetTickCount();

    for (;;) {
        xSemaphoreTake(xStatsMutex, portMAX_DELAY);
        print_report();
        xSemaphoreGive(xStatsMutex);

        vTaskDelayUntil(&xLastWake, period);
    }
}
#endif
