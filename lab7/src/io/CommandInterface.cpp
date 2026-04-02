#include "CommandInterface.h"

#include <Arduino.h>
#include <string.h>

CommandInterface::CommandInterface(Stream& primaryStream, Stream* secondaryStream)
    : _primaryStream(primaryStream), _secondaryStream(secondaryStream) {}

void CommandInterface::begin() {
    _length = 0;
    memset(_buffer, 0, sizeof(_buffer));
}

bool CommandInterface::pollLine(char* outBuffer, size_t outBufferSize) {
    if (readFromStream(_primaryStream, outBuffer, outBufferSize)) {
        return true;
    }

    if (_secondaryStream != nullptr) {
        return readFromStream(*_secondaryStream, outBuffer, outBufferSize);
    }

    return false;
}

bool CommandInterface::readFromStream(Stream& stream, char* outBuffer, size_t outBufferSize) {
    if (outBuffer == nullptr || outBufferSize == 0) {
        return false;
    }

    while (stream.available() > 0) {
        const char incoming = static_cast<char>(stream.read());

        if (incoming == '\r' || incoming == '\n') {
            if (_length == 0) {
                continue;
            }

            stream.write('\r');
            stream.write('\n');
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
                stream.write('\b');
                stream.write(' ');
                stream.write('\b');
            }
            continue;
        }

        if (_length < BufferSize - 1) {
            echoCharacter(stream, incoming);
            _buffer[_length++] = incoming;
            _buffer[_length] = '\0';
        }
    }

    return false;
}

void CommandInterface::echoCharacter(Stream& stream, char character) {
    stream.write(character);
}
