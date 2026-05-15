#include "CommandInterface.h"

#include <Arduino.h>
#include <string.h>

void CommandInterface::begin() {
    _length = 0;
    memset(_buffer, 0, sizeof(_buffer));
}

bool CommandInterface::pollLine(char* outBuffer, size_t outBufferSize) {
    if (outBuffer == nullptr || outBufferSize == 0) {
        return false;
    }

    while (Serial.available() > 0) {
        const char incoming = static_cast<char>(Serial.read());

        if (incoming == '\r' || incoming == '\n') {
            if (_length == 0) {
                continue;
            }

            Serial.write('\r');
            Serial.write('\n');
            _buffer[_length] = '\0';
            strncpy(outBuffer, _buffer, outBufferSize - 1);
            outBuffer[outBufferSize - 1] = '\0';
            _length = 0;
            _buffer[0] = '\0';
            return true;
        }

        if (incoming == '\b' || incoming == 127) {
            if (_length > 0) {
                _length--;
                _buffer[_length] = '\0';
                Serial.write('\b');
                Serial.write(' ');
                Serial.write('\b');
            }
            continue;
        }

        if (_length < BufferSize - 1) {
            Serial.write(incoming);
            _buffer[_length++] = incoming;
            _buffer[_length] = '\0';
        }
    }

    return false;
}
