# Lab 7 - Actuator Control with STDIO, LCD, Wokwi, and PlatformIO

## Goal

This lab implements a modular actuator control application for **Arduino Uno** using:
- a **binary actuator** represented by a digital indicator LED
- an **analog actuator** based on a **28BYJ-48 5V 5-wire stepper motor** driven through a **ULN2003 stepper driver board**
- **STDIO over Serial** for command input and structured reporting
- a **16x2 LCD** for live status display
- **Wokwi** for simulation and **PlatformIO** for build/run

Everything is written in English and the actuator logic is split into dedicated software drivers/modules.

## Implemented Requirements

- User commands arrive through **serial STDIO**.
- Binary control accepts `ON`, `OFF`, and `TOGGLE` requests.
- Binary requests are conditioned with **software debouncing** and stable-state validation.
- Analog control accepts a target percentage from `0` to `100`.
- Analog commands use **saturation**, **low-pass filtering**, and **soft ramping**.
- The LCD shows the current actuator state and active alerts.
- Serial reporting runs periodically every `500 ms`.
- Response latency stays below `100 ms` because control logic runs every `20 ms`.

## Command Interface

Open the serial monitor at `115200` baud and send one of these commands:

```text
help
status
bin on
bin off
bin toggle
ana <0-100>
report on
report off
```

Examples:

```text
bin on
ana 65
status
```

In Wokwi, use the built-in **Serial Monitor** for both command input and `printf` output.

## Software Architecture

- `src/app/ActuatorLabApp.*` - main application scheduler and command parser
- `src/actuators/BinaryActuator.*` - binary actuator driver
- `src/actuators/Stepper28BYJ48Actuator.*` - 28BYJ-48 stepper motor driver
- `src/signal/BinaryCommandConditioner.*` - debounce and stable-state validation
- `src/signal/AnalogCommandConditioner.*` - saturation, filtering, and ramping
- `src/io/StdioBridge.*` - `printf` output bridge for Serial
- `src/io/CommandInterface.*` - non-blocking serial line reader
- `src/io/LcdDisplay.*` - LCD driver for live status pages
- `src/config/AppConfig.h` - all pins and timing constants
- `src/main.cpp` - wiring and application entry point

## Control Flow Diagram

```mermaid
flowchart TD
    A[Serial command received] --> B[Command parser]
    B --> C[Binary conditioner]
    B --> D[Analog conditioner]
    C --> E[Binary actuator driver]
    D --> F[Stepper motor driver]
    E --> G[LCD + STDIO report]
    F --> G
```

## Binary Actuator State Diagram

```mermaid
stateDiagram-v2
    [*] --> Off
    Off --> PendingOn: bin on / toggle
    PendingOn --> On: stable for debounce time
    On --> PendingOff: bin off / toggle
    PendingOff --> Off: stable for debounce time
```

## Hardware Mapping

### LCD 16x2
- `RS -> D7`
- `E  -> D6`
- `D4 -> D5`
- `D5 -> D4`
- `D6 -> D3`
- `D7 -> D13`

### Actuators
- Binary actuator LED -> `D12`
- Stepper motor phase A- -> `D8`
- Stepper motor phase A+ -> `D9`
- Stepper motor phase B+ -> `D10`
- Stepper motor phase B- -> `D11`

### Wokwi Terminal
- Use the built-in Wokwi Serial Monitor
- It opens in `terminal` mode automatically
- Press Enter after each command
- The parser accepts both `CR` and `LF`, and supports backspace/delete

## Notes About the Simulation

- The **yellow LED** is the binary actuator indicator.
- The physical stepper target is **28BYJ-48 5V** driven through a **ULN2003 board**.
- The Wokwi diagram labels the `D8-D11` path as the ULN2003 driver section.
- Alerts are shown on the LCD and in `printf` status output.

## Build and Run

```bash
cd lab7
pio run
pio device monitor -b 115200
```

For Wokwi, open the `lab7` folder in VS Code with the Wokwi extension and run the simulation.

## Example Validation Scenario

1. Type `bin on` -> the binary actuator LED turns on after debounce validation.
2. Type `bin off` or `bin toggle` -> the binary actuator changes state.
3. Type `ana 30` or `ana 80` -> the stepper speed ramps smoothly to the requested level.
4. Watch the `printf` messages in the Wokwi terminal and the state pages on the LCD.
