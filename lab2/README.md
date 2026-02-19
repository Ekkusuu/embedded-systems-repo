# Lab 1.2 - User Interaction: STDIO + LCD + Keypad (Arduino Uno)

## Purpose
This project implements a modular user interaction application on Arduino Uno using:
- 4x4 Keypad for code input
- 16x2 LCD for text output
- Green and Red LEDs for status signaling
- STDIO-style functions (`printf`, `fprintf`, `fscanf`) for text I/O abstraction

## Functional Requirements Implemented
- Reads a 4-digit code from the keypad.
- Validates the code against a predefined value.
- Displays result on LCD.
- Turns on Green LED for valid code.
- Turns on Red LED for invalid code.
- Supports changing the access code directly from keypad (press `A`).
- Uses modular code structure, with separate files for each peripheral and application logic.

## Project Structure
- `src/config/AppConfig.h` - hardware pin mapping and constants
- `src/peripherals/LedController.*` - LED control
- `src/peripherals/LcdDisplay.*` - LCD abstraction
- `src/peripherals/KeypadInput.*` - keypad abstraction
- `src/io/StdioBridge.*` - STDIO streams for Serial, LCD output, and Keypad input
- `src/app/AccessControlApp.*` - application logic
- `src/main.cpp` - initialization and loop
- `diagram.json` - Wokwi circuit diagram
- `wokwi.toml` - Wokwi simulation configuration

## Block Diagram (Software)
1. `main.cpp` initializes all modules.
2. `AccessControlApp` requests a code.
3. `fscanf()` reads keypad keys through `StdioBridge` keypad stream.
4. `fprintf()` prints prompts and results to LCD stream.
5. Result controls LEDs via `LedController`.

## Hardware Connections

### LCD 16x2 (4-bit mode)
- RS -> D13
- E -> D12
- D4 -> D11
- D5 -> D10
- D6 -> D9
- D7 -> D8
- RW -> GND
- VSS -> GND
- VDD -> 5V
- VO -> GND (fixed contrast in simulation)

### Keypad 4x4
- R1 -> D7
- R2 -> D6
- R3 -> D5
- R4 -> D4
- C1 -> D3
- C2 -> D2
- C3 -> A3
- C4 -> A2

### LEDs
- Green LED anode -> A0 through 220 ohm resistor; cathode -> GND
- Red LED anode -> A1 through 220 ohm resistor; cathode -> GND

## Code and Naming Rules
- CamelCase naming style is used for classes/functions.
- Hardware and logic constants are centralized in `AppConfig.h` to avoid magic numbers.

## Build and Run (PlatformIO + Wokwi)
1. Build firmware:
   - `pio run`
2. Start simulation in Wokwi:
   - Open this folder in VS Code with Wokwi extension.
   - Run the simulation (it uses `diagram.json` and `wokwi.toml`).
3. Optional Serial Monitor:
   - `pio device monitor -b 115200`

## Demo Scenario
- Enter `1234` on keypad -> LCD shows `Access Granted`, Green LED turns on.
- Enter any other 4 keys -> LCD shows `Access Denied`, Red LED turns on.

## Change Code from Keypad
1. Press `A` to enter code change mode.
2. Enter current (old) 4-digit code.
3. If old code is correct, enter new 4-digit code.
4. LCD shows `Code Updated` and new code is used for next validations.
