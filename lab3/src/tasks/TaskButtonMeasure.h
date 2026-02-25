#pragma once

#include "../peripherals/ButtonInput.h"
#include "../peripherals/LedController.h"

// ---------------------------------------------------------------------------
// Task 1 – Button Detection & Duration Measurement
//
// Responsibility:
//   • Monitor the button state every 20 ms (bare-metal tick period).
//   • Debounce via ButtonInput driver.
//   • On press release:  record duration in gShared, set newPressAvailable.
//   • Light the green LED for short presses (< 500 ms) or the red LED for
//     long presses (≥ 500 ms); extinguish after IndicatorLedOnMs.
// ---------------------------------------------------------------------------

// Store references and call begin() on hardware.  Call from setup().
void task_button_init(ButtonInput& btn, LedController& leds);

// Bare-metal callable: runs every TaskButtonRec ms via the scheduler.
void task_button_run();

#ifdef PART_FREERTOS
// FreeRTOS task entry point.
void task_button_rtos(void* pvParams);
#endif
