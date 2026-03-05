#pragma once
#include <stdint.h>

// ============================================================
// SharedData – global state exchanged between the three tasks.
//
// Access rules (FreeRTOS):
//   lastPressDurationMs, newPressAvailable – written by Task 1,
//       read/cleared by Task 2; both access under xStatsMutex.
//   totalPresses … longDurationSumMs – written by Task 2,
//       read and reset by Task 3; both access under xStatsMutex.
// ============================================================
struct SharedData {
    // ── Set by Task 1, consumed by Task 2 ─────────────────────
    volatile uint32_t lastPressDurationMs; // duration of most recent press
    volatile bool     newPressAvailable;   // Task 1 flags; Task 2 clears

    // ── Accumulated by Task 2, reported and reset by Task 3 ───
    volatile uint32_t totalPresses;
    volatile uint32_t shortPresses;
    volatile uint32_t longPresses;
    volatile uint32_t shortDurationSumMs; // for computing short-press average
    volatile uint32_t longDurationSumMs;  // for computing long-press average
};

// Single global instance – defined in SharedData.cpp
extern SharedData gShared;
