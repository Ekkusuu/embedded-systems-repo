#include "KeypadInput.h"

KeypadInput::KeypadInput(char *keyMap, byte *rowPins, byte *columnPins,
                         uint8_t rowCount, uint8_t columnCount)
    : keypad(makeKeymap(keyMap), rowPins, columnPins, rowCount, columnCount) {}

char KeypadInput::getKey() { return keypad.getKey(); }

Keypad &KeypadInput::getRawKeypad() { return keypad; }
