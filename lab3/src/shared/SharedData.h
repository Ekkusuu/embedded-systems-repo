#pragma once
#include <stdint.h>

// ---------------------------------------------------------------------------
// SharedData – global state exchanged between the three tasks.
// In bare-metal mode no mutex is required because tasks are non-preemptive.
// In FreeRTOS mode every access is guarded by the mutex owned by each module.
// ---------------------------------------------------------------------------
struct SharedData {
    // ── Set by Task 1, consumed by Task 2 ───────────────────────────────────
    volatile uint32_t lastPressDurationMs;   // duration of most recent press
    volatile bool     newPressAvailable;     // Task 1 signals Task 2 via this

    // ── Accumulated by Task 2, read/cleared by Task 3 ────────────────────────
    volatile uint32_t totalPresses;
    volatile uint32_t shortPresses;
    volatile uint32_t longPresses;
    volatile uint32_t shortDurationSumMs;    // for short-press average
    volatile uint32_t longDurationSumMs;     // for long-press average
};

// One global instance – defined in SharedData.cpp
extern SharedData gShared;
