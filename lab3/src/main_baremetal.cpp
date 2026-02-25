// =============================================================================
// main_baremetal.cpp – Part 1: Non-preemptive bare-metal sequential scheduler
//
// Architecture:
//   ┌─ Timer1 ISR (1 ms) ──► os_seq_scheduler_run()
//   │       Task 1 (rec=20 ms, off=0):  button detect & duration measure
//   │       Task 2 (rec=20 ms, off=5):  statistics & non-blocking yellow blink
//   └─      Task 3 (rec=10 s,  off=10): STDIO statistics report + reset
// =============================================================================
#ifdef PART_BAREMETAL

#include <Arduino.h>

#include "config/AppConfig.h"
#include "peripherals/ButtonInput.h"
#include "peripherals/LedController.h"
#include "os/Scheduler.h"
#include "tasks/TaskButtonMeasure.h"
#include "tasks/TaskStatistics.h"
#include "tasks/TaskReport.h"

// ── Hardware instances (owned by main, shared via pointers after init) ────────
static ButtonInput   button(AppConfig::ButtonPin);
static LedController leds(AppConfig::LedGreenPin,
                           AppConfig::LedRedPin,
                           AppConfig::LedYellowPin);

void setup() {
    Serial.begin(AppConfig::SerialBaudRate);
    Serial.println(F("Lab3 – Bare-metal sequential scheduler"));
    Serial.println(F("Press the button; report every 10 s on serial."));

    // Initialise task modules (they store pointer references internally).
    task_button_init(button, leds);
    task_stats_init(leds);
    task_report_init();

    // Initialise and arm the Timer1-based sequential scheduler.
    os_seq_scheduler_setup();
}

void loop() {
    // Dispatch any pending tick – tasks run here, NOT inside the ISR.
    os_seq_scheduler_run();
}

#endif  // PART_BAREMETAL
