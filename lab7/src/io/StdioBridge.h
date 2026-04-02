#pragma once

#include <Stream.h>
#include <stdint.h>

namespace StdioBridge {

void begin(uint32_t baudRate);
void attachMirror(Stream* mirrorStream);

}  // namespace StdioBridge
