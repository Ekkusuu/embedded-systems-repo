#pragma once

// ============================================================
// Task 3 – Periodic STDIO Statistics Report (FreeRTOS)
//
// Responsibility:
//   • Every 10 s (vTaskDelayUntil, 10 ms initial offset):
//       – Acquire xStatsMutex to prevent races with Task 2.
//       – Print to Serial: total presses, short/long counts,
//         average short duration, average long duration.
//       – Reset all counters in gShared.
//       – Release xStatsMutex.
//
// Synchronisation primitives used:
//   xStatsMutex – protect gShared read + reset
// ============================================================

// Nothing to initialise; Serial is started in main. Call from setup().
void task_report_init();

// FreeRTOS task entry point.
void task_report_rtos(void* pvParams);
