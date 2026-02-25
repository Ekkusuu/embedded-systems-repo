#include "Scheduler.h"

#include <avr/interrupt.h>
#include <Arduino.h>

#include "../config/AppConfig.h"

// ── Forward declarations – implemented in tasks/ ─────────────────────────────
void task_button_run();
void task_stats_run();
void task_report_run();

// ── Internal state ────────────────────────────────────────────────────────────
volatile uint32_t sysTick_ms = 0;
volatile bool     tickFlag    = false;

// Task table: order also defines priority (higher index = lower priority).
// Populated by os_seq_scheduler_setup().
static Task_t tasks[TASK_COUNT];

// ── Timer1 ISR (1 ms) ────────────────────────────────────────────────────────
// Keep it minimal: increment the counter, set the flag.
ISR(TIMER1_COMPA_vect) {
    sysTick_ms++;
    tickFlag = true;
}

// ── Public API ────────────────────────────────────────────────────────────────
void os_seq_scheduler_task_init(Task_t* task,
                                void (*func)(void),
                                int rec,
                                int offset) {
    task->task_func = func;
    task->rec       = rec;
    task->offset    = offset;
    task->rec_cnt   = offset;  // first activation delayed by offset
}

void os_seq_scheduler_setup() {
    // ── Task table initialisation ────────────────────────────────────────────
    os_seq_scheduler_task_init(&tasks[TASK_BUTTON_ID],
                               task_button_run,
                               AppConfig::TaskButtonRec,
                               AppConfig::TaskButtonOff);

    os_seq_scheduler_task_init(&tasks[TASK_STATS_ID],
                               task_stats_run,
                               AppConfig::TaskStatsRec,
                               AppConfig::TaskStatsOff);

    os_seq_scheduler_task_init(&tasks[TASK_REPORT_ID],
                               task_report_run,
                               AppConfig::TaskReportRec,
                               AppConfig::TaskReportOff);

    // ── Timer1 – CTC mode, prescaler 64, OCR1A = 249 → 1 ms @ 16 MHz ────────
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);  // CTC, /64
    OCR1A  = 249;
    TIMSK1 = (1 << OCIE1A);
    sei();
}

void os_seq_scheduler_run() {
    // Atomic read-and-clear of the tick flag.
    noInterrupts();
    bool tick = tickFlag;
    if (tick) tickFlag = false;
    interrupts();

    if (!tick) return;  // no new tick – nothing to do

    // Walk the task table and dispatch every task whose counter expired.
    for (uint8_t i = 0; i < TASK_COUNT; i++) {
        if (--tasks[i].rec_cnt <= 0) {
            tasks[i].rec_cnt = tasks[i].rec;  // reload with recurrence
            tasks[i].task_func();             // execute task (non-blocking)
        }
    }
}
