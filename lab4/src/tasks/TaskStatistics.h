#pragma once

#include "../peripherals/LedController.h"

// ============================================================
// Task 2 – Counting, Statistics & Yellow-LED Blink (FreeRTOS)
//
// Responsibility:
//   • Block on xButtonSemaphore – woken by Task 1 on each press.
//   • Take xStatsMutex, read lastPressDurationMs, clear
//     newPressAvailable, update all counters, release mutex.
//   • Blink yellow LED:  5 blinks (short) or 10 blinks (long)
//     using vTaskDelay – blocking inside this task is fine since
//     it is event-driven and does not have a fixed period.
//
// Synchronisation primitives used:
//   xButtonSemaphore – wait for press event from Task 1
//   xStatsMutex      – protect gShared read + write
// ============================================================

// Store LED reference; calls LedController::begin(). Call from setup().
void task_stats_init(LedController& leds);

// FreeRTOS task entry point.
void task_stats_rtos(void* pvParams);
