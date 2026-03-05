#pragma once

#include "../peripherals/ButtonInput.h"
#include "../peripherals/LedController.h"

// ============================================================
// Task 1 – Button Detection & Duration Measurement (FreeRTOS)
//
// Responsibility:
//   • Monitor button state every 20 ms via vTaskDelayUntil.
//   • Debounce via ButtonInput non-blocking FSM.
//   • On release: record duration in gShared under xStatsMutex.
//   • Light green LED for short (< 500 ms) or red for long (≥ 500 ms);
//     extinguish after IndicatorLedOnMs without blocking.
//   • Give xButtonSemaphore to signal Task 2 of the new event.
//
// Synchronisation primitives used:
//   xStatsMutex      – protect gShared write
//   xButtonSemaphore – event signal to Task 2
// ============================================================

// Store hardware references; call from setup() before starting the scheduler.
void task_button_init(ButtonInput& btn, LedController& leds);

// FreeRTOS task entry point.
void task_button_rtos(void* pvParams);
