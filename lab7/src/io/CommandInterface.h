#pragma once

#include <stddef.h>

#include <Stream.h>

class CommandInterface {
public:
    CommandInterface(Stream& primaryStream, Stream* secondaryStream);

    void begin();
    bool pollLine(char* outBuffer, size_t outBufferSize);

private:
    bool readFromStream(Stream& stream, char* outBuffer, size_t outBufferSize);
    void echoCharacter(Stream& stream, char character);

    static constexpr size_t BufferSize = 48;

    Stream& _primaryStream;
    Stream* _secondaryStream;
    char _buffer[BufferSize] = {0};
    size_t _length = 0;
};
