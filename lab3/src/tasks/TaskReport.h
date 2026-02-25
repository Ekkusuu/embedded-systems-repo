#pragma once

// ---------------------------------------------------------------------------
// Task 3 – Periodic STDIO Statistics Report
//
// Responsibility:
//   • Every 10 seconds: print to Serial / STDIO:
//       – total presses
//       – number of short presses (< 500 ms)
//       – number of long presses  (≥ 500 ms)
//       – average short-press duration [ms]
//       – average long-press duration  [ms]
//   • After reporting, reset all counters in gShared.
// ---------------------------------------------------------------------------

void task_report_init();
void task_report_run();

#ifdef PART_FREERTOS
void task_report_rtos(void* pvParams);
#endif
