#pragma once

#include <stdint.h>

class LcdDisplay {
public:
    void begin();
    void showStatus(float setpointC,
                    bool sampleValid,
                    float temperatureC,
                    bool heaterOn,
                    uint8_t fanPercent);
};
