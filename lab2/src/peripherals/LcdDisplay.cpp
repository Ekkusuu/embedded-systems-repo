#include "LcdDisplay.h"

LcdDisplay::LcdDisplay(uint8_t rsPin, uint8_t enablePin, uint8_t d4Pin,
                       uint8_t d5Pin, uint8_t d6Pin, uint8_t d7Pin,
                       uint8_t columns, uint8_t rows)
    : lcd(rsPin, enablePin, d4Pin, d5Pin, d6Pin, d7Pin), lcdColumns(columns),
      lcdRows(rows) {}

void LcdDisplay::begin() { lcd.begin(lcdColumns, lcdRows); }

void LcdDisplay::clear() { lcd.clear(); }

void LcdDisplay::setCursor(uint8_t column, uint8_t row) {
  lcd.setCursor(column, row);
}

void LcdDisplay::print(const char *text) { lcd.print(text); }

void LcdDisplay::writeChar(char value) { lcd.write(value); }

LiquidCrystal &LcdDisplay::getRawLcd() { return lcd; }
