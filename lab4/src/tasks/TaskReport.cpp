// ============================================================
// TaskReport.cpp – Task 3 (FreeRTOS)
//
// Wakes every 10 s (vTaskDelayUntil) with a 10 ms initial offset.
// Reads and resets all gShared statistics under xStatsMutex, then
// prints a formatted report to Serial.
// ============================================================
#include "TaskReport.h"

#include <stdio.h>
#include <stdlib.h>   // ultoa() – lightweight uint32_t → string on AVR
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "../config/AppConfig.h"
#include "../shared/SharedData.h"

// Sync handle – defined in main.cpp
extern SemaphoreHandle_t xStatsMutex;

// ── Initialisation ────────────────────────────────────────────
void task_report_init() {
    // stdout is redirected to Serial in main.cpp; nothing to do here.
}

// ── Report helper (called under xStatsMutex) ─────────────────
static void print_report() {
    // Snapshot counters before reset
    uint32_t total    = gShared.totalPresses;
    uint32_t shorts   = gShared.shortPresses;
    uint32_t longs    = gShared.longPresses;
    uint32_t avgShort = (shorts > 0) ? (gShared.shortDurationSumMs / shorts) : 0;
    uint32_t avgLong  = (longs  > 0) ? (gShared.longDurationSumMs  / longs)  : 0;

    // Use printf (STDIO) – stdout is wired to Serial via fdev_setup_stream.
    // Numbers are converted with ultoa() then printed as %s to avoid pulling
    // in the AVR long-integer vfprintf formatter which needs ~300 bytes of
    // stack and causes a stack overflow in this task.
    char buf[11]; // max 10 decimal digits for uint32_t + null terminator

    printf("========================================\n");
    printf("  PERIODIC STATISTICS REPORT (10 s)\n");
    printf("========================================\n");
    ultoa(total,    buf, 10); printf("  Total presses     : %s\n", buf);
    ultoa(shorts,   buf, 10); printf("  Short presses     : %s\n", buf);
    ultoa(longs,    buf, 10); printf("  Long  presses     : %s\n", buf);
    ultoa(avgShort, buf, 10); printf("  Avg short dur [ms]: %s\n", buf);
    ultoa(avgLong,  buf, 10); printf("  Avg long  dur [ms]: %s\n", buf);
    printf("  (Counters reset)\n");
    printf("========================================\n");

    // Reset statistics for the next reporting window
    gShared.totalPresses       = 0;
    gShared.shortPresses       = 0;
    gShared.longPresses        = 0;
    gShared.shortDurationSumMs = 0;
    gShared.longDurationSumMs  = 0;
}

// ── FreeRTOS task ─────────────────────────────────────────────
void task_report_rtos(void* pvParams) {
    (void)pvParams;

    const TickType_t period = pdMS_TO_TICKS(AppConfig::ReportIntervalMs);
    const TickType_t offset = pdMS_TO_TICKS(AppConfig::TaskReportOffsetMs);

    // Apply initial offset so this task does not fire at tick 0
    vTaskDelay(offset);
    TickType_t xLastWake = xTaskGetTickCount();

    for (;;) {
        // Guard the read + reset with the shared mutex so Task 2 cannot
        // write new statistics while Task 3 is computing averages or resetting.
        xSemaphoreTake(xStatsMutex, portMAX_DELAY);
        print_report();
        xSemaphoreGive(xStatsMutex);

        // Precise 10 s wakeup (vTaskDelayUntil compensates for print time)
        vTaskDelayUntil(&xLastWake, period);
    }
}
