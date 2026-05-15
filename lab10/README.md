# Lab 9 - ON-OFF Temperature Control with Hysteresis

## Implemented Setup

- platform: `Arduino Uno`
- sensor: `10k NTC thermistor` on `A0`
- actuators: `heater resistor` on `D8` and `DC fan` through `L293D`
- user interface: `potentiometer on A1` + serial STDIO with `printf`
- scheduler: `sequential periodic tasks` with `millis()`

The physical idea is simple: a resistor is used as a small heater and the thermistor is placed very close to it, so the thermistor measures the local temperature rise. A fan is driven through `L293D`; when the assembly gets too hot, the fan speed ramps up gradually, then ramps back down as temperature falls.

## Control Law

- heater ON when `temperature <= setpoint - H`
- heater OFF when `temperature >= setpoint + H`
- otherwise keep previous state
- fan starts above `setpoint + 1.0 C`
- fan ramps to 100% by `setpoint + 10.0 C`
- fan slows back to 0% as temperature returns toward setpoint

Default values:

- setpoint range: `20.0 C .. 80.0 C` from potentiometer on `A1`
- startup setpoint: `28.0 C`
- hysteresis half-band: `1.0 C`

## Serial Commands

- `help`
- `status`

The setpoint is controlled live by the potentiometer.

## Serial Plotter Output

The firmware prints CSV lines through `printf`:

```text
setpoint,temperature,heater,fan
```

Example runtime lines:

```text
 35.0, 24.8,1,0
 35.0, 42.2,0,67
```

## Timing Model

- acquisition task: `100 ms`
- control task: `250 ms`
- report task: `500 ms`

## Wokwi Note

Wokwi does not model real thermal coupling between the heater resistor and the thermistor. The diagram reflects the intended real hardware placement, but in simulation you still need to adjust the thermistor temperature manually.

## Build

```bash
cd lab9
pio run
```
