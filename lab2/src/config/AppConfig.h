#pragma once

#include <Arduino.h>

namespace AppConfig {
constexpr uint32_t SerialBaudRate = 115200;
constexpr uint8_t CodeLength = 4;
constexpr uint8_t LcdColumns = 16;
constexpr uint8_t LcdRows = 2;

constexpr char ValidCode[CodeLength + 1] = "1234";

constexpr uint8_t GreenLedPin = A0;
constexpr uint8_t RedLedPin = A1;

constexpr uint8_t LcdRsPin = 13;
constexpr uint8_t LcdEnablePin = 12;
constexpr uint8_t LcdD4Pin = 11;
constexpr uint8_t LcdD5Pin = 10;
constexpr uint8_t LcdD6Pin = 9;
constexpr uint8_t LcdD7Pin = 8;

constexpr uint8_t KeypadRowCount = 4;
constexpr uint8_t KeypadColumnCount = 4;

extern byte KeypadRows[KeypadRowCount];
extern byte KeypadColumns[KeypadColumnCount];

extern char KeyLayout[KeypadRowCount][KeypadColumnCount];
}
