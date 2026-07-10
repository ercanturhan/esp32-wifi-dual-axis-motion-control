#include "RelayController.h"

RelayController::RelayController()
    : _id(0), _pin(-1), _activeHigh(true), _active(false), _durationMs(0), _startedAt(0) {}

void RelayController::begin(int id, int pin, bool activeHigh) {
    _id = id;
    _pin = pin;
    _activeHigh = activeHigh;
    _active = false;
    _durationMs = 0;
    _startedAt = 0;

    pinMode(_pin, OUTPUT);
    writeRelay(false);
}

void RelayController::update() {
    if (!_active) return;
    if (_durationMs == 0) return;

    if (millis() - _startedAt >= _durationMs) {
        turnOff();
    }
}

void RelayController::triggerMilliseconds(uint32_t durationMs) {
    if (durationMs == 0) {
        turnOff();
        return;
    }

    _durationMs = durationMs;
    _startedAt = millis();
    _active = true;
    writeRelay(true);
}

void RelayController::turnOn() {
    _durationMs = 0;
    _startedAt = millis();
    _active = true;
    writeRelay(true);
}

void RelayController::turnOff() {
    _active = false;
    _durationMs = 0;
    _startedAt = 0;
    writeRelay(false);
}

uint32_t RelayController::getRemainingMilliseconds() const {
    if (!_active) return 0;
    if (_durationMs == 0) return 0;

    unsigned long elapsed = millis() - _startedAt;
    if (elapsed >= _durationMs) return 0;

    return (uint32_t)(_durationMs - elapsed);
}

void RelayController::writeRelay(bool active) {
    if (_pin < 0) return;
    digitalWrite(_pin, active == _activeHigh ? HIGH : LOW);
}
