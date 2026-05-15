#pragma once

#include <stddef.h>

class CommandInterface {
public:
    void begin();
    bool pollLine(char* outBuffer, size_t outBufferSize);

private:
    static constexpr size_t BufferSize = 32;

    char _buffer[BufferSize] = {0};
    size_t _length = 0;
};
