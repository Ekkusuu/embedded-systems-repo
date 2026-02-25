#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------------
// Sequential bare-metal scheduler
// ─ Timer1 fires every 1 ms and sets a tick flag.
// ─ os_seq_scheduler_run() must be called from loop(); it dispatches every
//   task whose countdown reaches zero (non-preemptive, non-blocking).
// ─ Each task descriptor holds: function pointer, recurrence [ms], offset,
//   and a runtime countdown counter (following the theory pattern).
// ---------------------------------------------------------------------------

// Task descriptor (as described in the lab theory).
struct Task_t {
    void (*task_func)(void);  // pointer to the task function
    int   rec;                // recurrence period in ms (scheduler ticks)
    int   offset;             // first-activation offset in ms
    int   rec_cnt;            // runtime countdown
};

// Identifiers used to index the tasks[] array.
enum TaskId : uint8_t {
    TASK_BUTTON_ID = 0,
    TASK_STATS_ID,
    TASK_REPORT_ID,
    TASK_COUNT
};

// ---------------------------------------------------------------------------
// API
// ---------------------------------------------------------------------------

// Configure Timer1 for 1 ms CTC interrupts and initialise all task counters
// with their offset values.  Call once from setup().
void os_seq_scheduler_setup();

// Initialise a single task descriptor (helper used by setup).
void os_seq_scheduler_task_init(Task_t* task,
                                void (*func)(void),
                                int rec,
                                int offset);

// Call from loop().  Returns immediately if no tick has occurred.
// Dispatches every task whose countdown has expired (in priority order).
void os_seq_scheduler_run();

// Current system time in milliseconds (incremented by the ISR).
// Declared volatile so tasks can safely read it.
extern volatile uint32_t sysTick_ms;
