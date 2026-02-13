#include <Arduino.h>
#include "SerialComm.h"
#include <string.h>

// ── STDIO bridge ─────────────────────────────
static FILE uartStream;

static int uartPutChar(char c, FILE* stream) {
    (void)stream;
    Serial.write((uint8_t)c);
    return 0;
}

static int uartGetChar(FILE* stream) {
    (void)stream;
    while (!Serial.available()) {}
    return Serial.read();
}

static void stdioInit() {
    fdev_setup_stream(&uartStream, uartPutChar, uartGetChar, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uartStream;
}
// ─────────────────────────────────────────────

SerialComm::SerialComm() : _index(0) {
    memset(_buffer, 0, CMD_BUFFER_SIZE);
}

void SerialComm::begin(unsigned long baudRate) {
    Serial.begin(baudRate);
    while (!Serial) {}
    stdioInit();
    printf("=== LED Control System ===\r\n");
    printf("Commands: \"led on\" | \"led off\"\r\n");
}

bool SerialComm::readLine() {
    memset(_buffer, 0, CMD_BUFFER_SIZE);
    if (scanf(" %31[^\r\n]", _buffer) == 1) {
        return true;
    }
    return false;
}

const char* SerialComm::getCommand() const {
    return _buffer;
}