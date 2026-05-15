#include "StdioBridge.h"

#include <Arduino.h>
#include <stdio.h>

namespace {

int serial_putchar(char c, FILE* stream) {
    (void)stream;
    if (c == '\n') {
        Serial.write('\r');
    }
    Serial.write(c);
    return 0;
}

FILE gSerialOut;

}  // namespace

namespace StdioBridge {

void begin(uint32_t baudRate) {
    Serial.begin(baudRate);
    fdev_setup_stream(&gSerialOut, serial_putchar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &gSerialOut;
}

}  // namespace StdioBridge
