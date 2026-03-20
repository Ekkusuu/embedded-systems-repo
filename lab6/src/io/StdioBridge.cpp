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

FILE gStdioUart;

}  // namespace

namespace StdioBridge {

void begin(uint32_t baudRate) {
    Serial.begin(baudRate);
    fdev_setup_stream(&gStdioUart, serial_putchar, nullptr, _FDEV_SETUP_WRITE);
    stdout = &gStdioUart;
}

}  // namespace StdioBridge
