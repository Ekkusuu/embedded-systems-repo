#pragma once
#include <Arduino.h>

// ---------------------------------------------------------------------------
// LedController – ECAL-level driver for three indicator LEDs.
//   Green  – short press  (< 500 ms)
//   Red    – long press   (≥ 500 ms)
//   Yellow – statistics blink
// ---------------------------------------------------------------------------
class LedController {
public:
    LedController(uint8_t greenPin, uint8_t redPin, uint8_t yellowPin);

    void begin();

    void setGreen(bool on);
    void setRed(bool on);
    void setYellow(bool on);

    void indicateShortPress();  // green ON, red OFF
    void indicateLongPress();   // red ON, green OFF
    void clearIndicator();      // both green and red OFF

private:
    uint8_t greenPin_;
    uint8_t redPin_;
    uint8_t yellowPin_;
};
