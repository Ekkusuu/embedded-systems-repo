# Lab 11 - Part 1: Button-LED Finite-State Machine

## Requirement

Implement an embedded finite-state machine that controls an LED using a button:

- state `OFF`: LED is off
- state `ON`: LED is on
- each valid button press toggles the state
- button debounce is required
- current state is reported in real time through `printf` using STDIO

## Pinout

Lab 9/10 already used `A0`, `A1`, `D2`, `D3`, `D4`, `D5`, `D6`, `D7`, `D8`, `D10`, `D11`, and `D13`.

This lab avoids those GPIOs for the new hardware:

- `D9` -> LED through `220 ohm` resistor
- `D12` -> pushbutton to `GND`
- `D0/D1` -> USB serial terminal for `printf`

The existing Lab 9/10 LCD may stay connected and is initialized by this firmware:

- `D7` -> LCD `RS`
- `D6` -> LCD `E`
- `D5` -> LCD `D4`
- `D4` -> LCD `D5`
- `D3` -> LCD `D6`
- `D13` -> LCD `D7`

The button uses `INPUT_PULLUP`, so:

- released = `HIGH`
- pressed = `LOW`

Wire the button exactly as `D12 -> button -> GND`.

## Behaviour

The FSM has two states:

- `OFF`
- `ON`

On every debounced rising press event, the state toggles:

- `OFF -> ON`
- `ON -> OFF`

Debounce delay: `40 ms`.

## Build

```bash
cd lab11
pio run
```
