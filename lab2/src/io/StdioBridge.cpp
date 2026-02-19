#include "StdioBridge.h"

LiquidCrystal *StdioBridge::lcd = nullptr;
Keypad *StdioBridge::keypad = nullptr;
uint8_t StdioBridge::lcdColumn = 0;
uint8_t StdioBridge::lcdRow = 0;

FILE StdioBridge::serialFile;
FILE StdioBridge::lcdFile;
FILE StdioBridge::keypadFile;

void StdioBridge::begin(uint32_t baudRate, LiquidCrystal *lcdRef,
                        Keypad *keypadRef) {
  lcd = lcdRef;
  keypad = keypadRef;
  lcdColumn = 0;
  lcdRow = 0;

  Serial.begin(baudRate);
  while (!Serial) {
  }

  fdev_setup_stream(&serialFile, serialPutChar, nullptr, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&lcdFile, lcdPutChar, nullptr, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&keypadFile, nullptr, keypadGetChar, _FDEV_SETUP_READ);

  stdout = &serialFile;
}

void StdioBridge::syncLcdCursor(uint8_t column, uint8_t row) {
  lcdColumn = column;
  lcdRow = row;

  if (lcd != nullptr) {
    lcd->setCursor(lcdColumn, lcdRow);
  }
}

FILE *StdioBridge::serialStream() { return &serialFile; }

FILE *StdioBridge::lcdStream() { return &lcdFile; }

FILE *StdioBridge::keypadStream() { return &keypadFile; }

int StdioBridge::serialPutChar(char character, FILE *stream) {
  (void)stream;

  if (character == '\n') {
    Serial.write('\r');
  }

  Serial.write(character);
  return 0;
}

int StdioBridge::lcdPutChar(char character, FILE *stream) {
  (void)stream;

  if (lcd == nullptr) {
    return 0;
  }

  constexpr uint8_t LcdColumns = 16;
  constexpr uint8_t LcdRows = 2;

  if (character == '\n') {
    lcdColumn = 0;
    lcdRow = (lcdRow + 1) % LcdRows;
    lcd->setCursor(lcdColumn, lcdRow);
    return 0;
  }

  lcd->write(character);
  lcdColumn++;

  if (lcdColumn >= LcdColumns) {
    lcdColumn = 0;
    lcdRow = (lcdRow + 1) % LcdRows;
    lcd->setCursor(lcdColumn, lcdRow);
  }

  return 0;
}

int StdioBridge::keypadGetChar(FILE *stream) {
  (void)stream;

  if (keypad == nullptr) {
    return -1;
  }

  char keyValue = NO_KEY;
  while (keyValue == NO_KEY) {
    keyValue = keypad->getKey();
  }

  return keyValue;
}
