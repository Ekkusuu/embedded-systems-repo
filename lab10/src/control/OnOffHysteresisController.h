#pragma once

class OnOffHysteresisController {
public:
    OnOffHysteresisController(float setpoint, float halfBand);

    void setSetpoint(float setpoint);
    float setpoint() const;

    bool update(float measuredValue);
    bool output() const;
    float thresholdOn() const;
    float thresholdOff() const;

private:
    float _setpoint;
    float _halfBand;
    bool _output;
};
