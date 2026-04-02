#include "BinaryCommandConditioner.h"

BinaryCommandConditioner::BinaryCommandConditioner(uint32_t debounceMs)
    : _debounceMs(debounceMs) {}

void BinaryCommandConditioner::submit(bool requestedState, uint32_t nowMs) {
    _pendingState = requestedState;
    _pendingSinceMs = nowMs;
    _hasPending = (_pendingState != _stableState);
}

bool BinaryCommandConditioner::tick(uint32_t nowMs, bool* committedState) {
    if (!_hasPending) {
        return false;
    }

    if ((nowMs - _pendingSinceMs) < _debounceMs) {
        return false;
    }

    _stableState = _pendingState;
    _hasPending = false;

    if (committedState != nullptr) {
        *committedState = _stableState;
    }

    return true;
}

bool BinaryCommandConditioner::getStableState() const {
    return _stableState;
}

bool BinaryCommandConditioner::hasPendingChange() const {
    return _hasPending;
}
