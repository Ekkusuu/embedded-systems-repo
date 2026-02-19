#pragma once

#include <LiquidCrystal.h>

class LcdDisplay {
public:
  LcdDisplay(uint8_t rsPin, uint8_t enablePin, uint8_t d4Pin, uint8_t d5Pin,
             uint8_t d6Pin, uint8_t d7Pin, uint8_t columns, uint8_t rows);

  void begin();
  void clear();
  void setCursor(uint8_t column, uint8_t row);
  void print(const char *text);
  void writeChar(char value);
  LiquidCrystal &getRawLcd();

private:
  LiquidCrystal lcd;
  uint8_t lcdColumns;
  uint8_t lcdRows;
};
