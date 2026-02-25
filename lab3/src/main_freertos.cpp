// =============================================================================
// main_freertos.cpp – Part 2: FreeRTOS preemptive multitasking
//
// Architecture:
//   Task 1 – task_button_rtos   (prio 2, 20 ms period via vTaskDelayUntil)
//   Task 2 – task_stats_rtos    (prio 2, event-driven via binary semaphore)
//   Task 3 – task_report_rtos   (prio 1, 10 s period via vTaskDelayUntil)
//
// Synchronisation:
//   xButtonSemaphore – binary semaphore: Task 1 gives, Task 2 takes
//   xStatsMutex      – mutex protecting gShared fields
// =============================================================================
#ifdef PART_FREERTOS

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#include "config/AppConfig.h"
#include "peripherals/ButtonInput.h"
#include "peripherals/LedController.h"
#include "tasks/TaskButtonMeasure.h"
#include "tasks/TaskStatistics.h"
#include "tasks/TaskReport.h"

// ── Synchronisation handles (defined here, extern'd in task files) ────────────
SemaphoreHandle_t xButtonSemaphore = nullptr;
SemaphoreHandle_t xStatsMutex      = nullptr;

// ── Hardware instances ────────────────────────────────────────────────────────
static ButtonInput   button(AppConfig::ButtonPin);
static LedController leds(AppConfig::LedGreenPin,
                           AppConfig::LedRedPin,
                           AppConfig::LedYellowPin);

void setup() {
    Serial.begin(AppConfig::SerialBaudRate);
    Serial.println(F("Lab3 – FreeRTOS preemptive scheduler"));
    Serial.println(F("Press the button; report every 10 s on serial."));

    // Create synchronisation primitives.
    xButtonSemaphore = xSemaphoreCreateBinary();
    xStatsMutex      = xSemaphoreCreateMutex();

    // Initialise task modules.
    task_button_init(button, leds);
    task_stats_init(leds);
    task_report_init();

    // Create FreeRTOS tasks.
    xTaskCreate(task_button_rtos, "BTN",    AppConfig::StackButton, nullptr, 2, nullptr);
    xTaskCreate(task_stats_rtos,  "STATS",  AppConfig::StackStats,  nullptr, 2, nullptr);
    xTaskCreate(task_report_rtos, "REPORT", AppConfig::StackReport, nullptr, 1, nullptr);

    // Hand control to the FreeRTOS scheduler.
    vTaskStartScheduler();

    // Should never reach here.
    for (;;);
}

void loop() {
    // FreeRTOS is running; loop() is never called after vTaskStartScheduler().
}

#endif  // PART_FREERTOS
