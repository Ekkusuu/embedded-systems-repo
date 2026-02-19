#pragma once

#include <Keypad.h>

class KeypadInput {
public:
  KeypadInput(char *keyMap, byte *rowPins, byte *columnPins, uint8_t rowCount,
              uint8_t columnCount);

  char getKey();
  Keypad &getRawKeypad();

private:
  Keypad keypad;
};
