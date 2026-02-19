#pragma once

#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <stdio.h>

class StdioBridge {
public:
  static void begin(uint32_t baudRate, LiquidCrystal *lcdRef, Keypad *keypadRef);
  static void syncLcdCursor(uint8_t column, uint8_t row);

  static FILE *serialStream();
  static FILE *lcdStream();
  static FILE *keypadStream();

private:
  static int serialPutChar(char character, FILE *stream);
  static int lcdPutChar(char character, FILE *stream);
  static int keypadGetChar(FILE *stream);

  static LiquidCrystal *lcd;
  static Keypad *keypad;
  static uint8_t lcdColumn;
  static uint8_t lcdRow;

  static FILE serialFile;
  static FILE lcdFile;
  static FILE keypadFile;
};
