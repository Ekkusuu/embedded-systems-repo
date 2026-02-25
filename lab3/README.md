# Lab 3 – Button Duration Monitor with Multitasking

**Embedded Systems – Laboratory Work #3**  
FAF-233 · Sava Luchian

---

## Overview

Button press duration monitoring system with LED signalling and periodic STDIO statistics, implemented in **two variants**:

| Part | Model | File |
|------|-------|------|
| 1 | Bare-metal non-preemptive sequential scheduler | `main_baremetal.cpp` |
| 2 | FreeRTOS preemptive tasks with semaphores & mutex | `main_freertos.cpp` |

---

## Hardware

| Component | Arduino Uno Pin |
|-----------|----------------|
| Push-button (INPUT_PULLUP) | D2 |
| Green LED + 220 Ω | D10 |
| Red LED + 220 Ω | D11 |
| Yellow LED + 220 Ω | D12 |

**Button wiring:** one leg to D2, other leg to GND (Arduino internal pull-up active).

---

## Task Logic

### Task 1 – Button Detection & Duration Measurement (20 ms period)
- Reads button with debounce (20 ms stable window).
- On release: measures press duration, stores it in `gShared`.
- **Green LED** → short press (< 500 ms).
- **Red LED** → long press (≥ 500 ms). LED stays on for 800 ms.

### Task 2 – Counting & Statistics (20 ms period, 5 ms offset)
- Detects `gShared.newPressAvailable` flag.
- Increments total / short / long press counters and duration sums.
- **Yellow LED blink**: 5 blinks (short) or 10 blinks (long), 100 ms period.
  - Bare-metal: non-blocking state machine.
  - FreeRTOS: blocking `vTaskDelay` loop inside the task.

### Task 3 – Periodic STDIO Report (10 000 ms period, 10 ms offset)
- Prints to Serial every 10 seconds:
  - total presses, short/long counts, average short/long duration.
- Resets all counters after printing.

---

## Scheduler Details (Part 1 – Bare Metal)

```
Timer1 CTC, prescaler 64, OCR1A = 249 → 1 ms tick @ 16 MHz

Task ID | Function          | Period | Offset
--------|-------------------|--------|--------
0       | task_button_run   | 20 ms  |  0 ms
1       | task_stats_run    | 20 ms  |  5 ms
2       | task_report_run   | 10000ms| 10 ms
```

CPU tick utilisation per 1 ms tick (typical):  
Task 1 (20 ms slot): ~20 µs | Task 2 (20 ms slot): ~20 µs | Task 3 (rare): ~100 µs  
→ well under 70% limit.

---

## Synchronisation Details (Part 2 – FreeRTOS)

| Primitive | Name | Owner (Give) | Waiter (Take) |
|-----------|------|-------------|---------------|
| Binary semaphore | `xButtonSemaphore` | Task 1 | Task 2 |
| Mutex | `xStatsMutex` | all tasks | all tasks |

- Task 1 sets `gShared` fields under mutex, then gives the semaphore.
- Task 2 blocks on semaphore, takes mutex to read/update `gShared`.
- Task 3 takes mutex before reading and resetting counters.

---

## Build & Run

```bash
# Part 1 – bare-metal
pio run -e baremetal
pio run -e baremetal --target upload

# Part 2 – FreeRTOS
pio run -e freertos
pio run -e freertos --target upload

# Monitor serial output
pio device monitor --baud 115200
```

### Wokwi Simulation
Open the project in VS Code with the Wokwi extension installed.  
The simulator uses the **baremetal** firmware by default (`wokwi.toml`).

---

## Project Structure

```
lab3/
├── platformio.ini          ← two environments: baremetal / freertos
├── diagram.json            ← Wokwi circuit
├── wokwi.toml
├── src/
│   ├── main_baremetal.cpp  ← Part 1 entry point
│   ├── main_freertos.cpp   ← Part 2 entry point
│   ├── config/
│   │   └── AppConfig.h     ← all pins and timing constants
│   ├── shared/
│   │   ├── SharedData.h    ← inter-task shared state
│   │   └── SharedData.cpp
│   ├── peripherals/
│   │   ├── ButtonInput.h/cpp   ← ECAL debounce driver
│   │   └── LedController.h/cpp ← ECAL LED driver
│   ├── os/
│   │   ├── Scheduler.h/cpp     ← bare-metal sequential scheduler + Timer1 ISR
│   ├── tasks/
│   │   ├── TaskButtonMeasure.h/cpp  ← Task 1
│   │   ├── TaskStatistics.h/cpp     ← Task 2
│   │   └── TaskReport.h/cpp         ← Task 3
└── report/
    └── main.tex
```
