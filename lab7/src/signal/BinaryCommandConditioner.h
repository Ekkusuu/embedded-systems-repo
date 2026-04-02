#pragma once

#include <stdint.h>

class BinaryCommandConditioner {
public:
    explicit BinaryCommandConditioner(uint32_t debounceMs);

    void submit(bool requestedState, uint32_t nowMs);
    bool tick(uint32_t nowMs, bool* committedState);
    bool getStableState() const;
    bool hasPendingChange() const;

private:
    const uint32_t _debounceMs;
    bool _stableState = false;
    bool _pendingState = false;
    bool _hasPending = false;
    uint32_t _pendingSinceMs = 0;
};
