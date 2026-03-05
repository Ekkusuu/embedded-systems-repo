// ============================================================
// main.cpp – Lab 4: FreeRTOS Preemptive Multitasking
//
// System architecture:
//
//   Task 1 – task_button_rtos  (prio 2, 20 ms period, vTaskDelayUntil)
//     Monitors the button state, debounces, measures press duration,
//     drives green/red LED, writes gShared under xStatsMutex, and
//     gives xButtonSemaphore to wake Task 2.
//
//   Task 2 – task_stats_rtos   (prio 2, event-driven via semaphore)
//     Waits on xButtonSemaphore, reads gShared under xStatsMutex,
//     updates statistics counters, blinks the yellow LED with vTaskDelay.
//
//   Task 3 – task_report_rtos  (prio 1, 10 s period, vTaskDelayUntil)
//     Acquires xStatsMutex, prints statistics and resets counters every 10 s.
//
// Synchronisation:
//   xButtonSemaphore – binary semaphore: Task 1 gives → Task 2 takes
//   xStatsMutex      – mutex: guards gShared access in Tasks 1, 2 and 3
// ============================================================

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdio.h>

#include "config/AppConfig.h"
#include "peripherals/ButtonInput.h"
#include "peripherals/LedController.h"
#include "tasks/TaskButtonMeasure.h"
#include "tasks/TaskStatistics.h"
#include "tasks/TaskReport.h"

// ── STDIO → Serial bridge ──────────────────────────────────────
// Routes stdout (and therefore printf) to the hardware UART so all
// tasks can use printf() for output without touching Serial directly.
static int serial_putchar(char c, FILE* stream) {
    (void)stream;
    if (c == '\n') Serial.write('\r');  // translate LF → CR+LF for serial terminals
    Serial.write(c);
    return 0;
}
static FILE stdio_uart;

// ── Synchronisation handles (extern'd in every task .cpp) ─────
SemaphoreHandle_t xButtonSemaphore = nullptr;
SemaphoreHandle_t xStatsMutex      = nullptr;

// ── Hardware instances ─────────────────────────────────────────
static ButtonInput   button(AppConfig::ButtonPin);
static LedController leds(AppConfig::LedGreenPin,
                           AppConfig::LedRedPin,
                           AppConfig::LedYellowPin);

void setup() {
    Serial.begin(AppConfig::SerialBaudRate);

    // Redirect stdout to Serial so printf() works as STDIO throughout
    fdev_setup_stream(&stdio_uart, serial_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &stdio_uart;

    printf("Lab 4 - FreeRTOS Preemptive Scheduler\n");
    printf("Wiring: BTN=D2  GREEN=D9  RED=D10  YELLOW=D11\n");
    printf("Press the button; report prints every 10 s.\n");
    printf("\n");

    // 1. Create synchronisation primitives before starting tasks
    xButtonSemaphore = xSemaphoreCreateBinary(); // Task1→Task2 event signal
    xStatsMutex      = xSemaphoreCreateMutex();  // protect gShared

    // 2. Initialise hardware modules
    task_button_init(button, leds);  // configures button pin
    task_stats_init(leds);           // configures LED pins (all three)
    task_report_init();              // Serial already started above

    // 3. Create FreeRTOS tasks
    xTaskCreate(task_button_rtos, "BTN",    AppConfig::StackButton,
                nullptr, AppConfig::PrioButton, nullptr);
    xTaskCreate(task_stats_rtos,  "STATS",  AppConfig::StackStats,
                nullptr, AppConfig::PrioStats,  nullptr);
    xTaskCreate(task_report_rtos, "REPORT", AppConfig::StackReport,
                nullptr, AppConfig::PrioReport, nullptr);

    // 4. Hand control to the FreeRTOS scheduler – never returns
    vTaskStartScheduler();

    // Should never reach here
    for (;;);
}

void loop() {
    // FreeRTOS scheduler is running; loop() is never called after
    // vTaskStartScheduler() returns control to the kernel.
}
