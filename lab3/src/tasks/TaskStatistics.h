#pragma once

#include "../peripherals/LedController.h"

// ---------------------------------------------------------------------------
// Task 2 – Counting, Statistics & Yellow-LED Blink
//
// Responsibility:
//   • Detect newPressAvailable set by Task 1.
//   • Increment totalPresses, shortPresses / longPresses counters.
//   • Accumulate short / long duration sums (for averages in Task 3).
//   • Drive yellow LED blink:  5 blinks for short press, 10 for long press.
//     The blink is non-blocking in bare-metal mode (state machine).
//     In FreeRTOS mode the task blocks with vTaskDelay inside a loop.
// ---------------------------------------------------------------------------

void task_stats_init(LedController& leds);
void task_stats_run();

#ifdef PART_FREERTOS
void task_stats_rtos(void* pvParams);
#endif
