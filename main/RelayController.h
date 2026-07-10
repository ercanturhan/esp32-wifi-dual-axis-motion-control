#ifndef RELAY_CONTROLLER_H
#define RELAY_CONTROLLER_H

#include <Arduino.h>

class RelayController {
public:
    RelayController();

    void begin(int id, int pin, bool activeHigh);
    void update();
    void triggerMilliseconds(uint32_t durationMs);
    void turnOn();
    void turnOff();

    int getId() const { return _id; }
    int getPin() const { return _pin; }
    bool isActive() const { return _active; }
    uint32_t getDurationMilliseconds() const { return _durationMs; }
    uint32_t getRemainingMilliseconds() const;

private:
    int _id;
    int _pin;
    bool _activeHigh;
    bool _active;
    uint32_t _durationMs;
    unsigned long _startedAt;

    void writeRelay(bool active);
};

#endif // RELAY_CONTROLLER_H
