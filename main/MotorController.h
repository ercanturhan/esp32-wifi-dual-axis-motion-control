#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "FastAccelStepper.h"
#include "Config.h"

// Homing States
enum HomingState {
    HOME_IDLE = 0,
    HOME_INIT,
    HOME_FAST_BACKWARD,
    HOME_FAST_SWITCH_CONFIRM,
    HOME_BACKOFF_SETTLE,
    HOME_BACKING_OFF,
    HOME_BACKOFF_CLEARANCE,
    HOME_SLOW_APPROACH_SETTLE,
    HOME_SLOW_APPROACH,
    HOME_SLOW_SWITCH_CONFIRM,
    HOME_FAILED
};

class MotorController {
public:
    MotorController();
    
    // Initialize the motor controller
    void begin(int id, FastAccelStepperEngine& engine, MotorConfig& config, 
                int stepPin, int dirPin, int enablePin, int homePin,
                int m0Pin, int m1Pin, int m2Pin);
    
    // Update function called in main loop
    void update();
    
    // Movement commands
    bool moveTo(float targetUnit);
    bool moveAbsolute(long steps);
    bool moveRelative(long steps, bool clampToLimits = false);
    void stop();
    void startHoming();
    
    // Enable/Disable DRV8825 driver
    void enableDriver(bool enable);
    bool isDriverEnabled() const { return _driverEnabled; }
    
    // Settings update
    void updateConfig(const MotorConfig& newConfig);
    bool setRuntimeSpeed(float speedStepsPerSecond);
    
    // Getters for status & telemetry
    int getId() const { return _id; }
    long getCurrentPosition() const;
    long getTargetPosition() const;
    float getCurrentPositionUnit() const;
    float getTargetPositionUnit() const;
    long getRemainingDistance() const;
    float getRemainingDistanceUnit() const;
    int getProgressPercent() const;
    float getSpeed() const;
    float getAcceleration() const;
    bool isRunning() const;
    bool isHoming() const;
    bool isHomeCompleted() const { return _homeCompleted; }
    bool hasHomingFailed() const { return _homingState == HOME_FAILED; }
    bool isLimitSwitchActive() const;
    bool wasLastMoveBlockedByHomeSwitch() const { return _lastMoveBlockedByHomeSwitch; }
    bool consumeHomeLimitEvent();
    
    // Conversion helpers
    double getStepsPerUnit() const { return _stepsPerUnit; }
    long unitsToSteps(float units) const;
    float stepsToUnits(long steps) const;
    
    // Reset position to zero manually
    void resetPosition();

    // Set position manually to a specific step count
    void setCurrentPosition(long pos);

    // Enable/Disable auto-enable timeout feature
    void setEnableTimeout(bool active, uint32_t seconds);

private:
    int _id;
    FastAccelStepper* _stepper;
    MotorConfig _config;
    
    int _stepPin;
    int _dirPin;
    int _enablePin;
    int _homePin;
    int _m0Pin;
    int _m1Pin;
    int _m2Pin;
    
    // Homing State Machine
    HomingState _homingState;
    bool _homeCompleted;
    unsigned long _homingStartTime;
    unsigned long _homingStateStartTime;

    // Homing decisions use a debounced switch. Normal travel protection still
    // reads the raw input so a limit hit stops the motor without added delay.
    bool _homeSwitchRaw;
    bool _homeSwitchStable;
    unsigned long _homeSwitchChangedAt;
    
    // Driver Enable / Timeout Management
    bool _driverEnabled;
    bool _timeoutActive;
    uint32_t _timeoutDurationMs;
    unsigned long _lastActiveTime;

    // The home switch protects the negative (homing) direction during normal moves.
    bool _lastMoveBlockedByHomeSwitch;
    bool _homeLimitEventPending;

    // Last target accepted by this controller. FastAccelStepper publishes its
    // target asynchronously, so rapid jog presses need a synchronous target.
    long _plannedTarget;
    
    // Conversion factor
    double _stepsPerUnit;
    void calculateStepsPerUnit();
    
    // Apply speed/accel from config
    void applySpeedProfile();

    void updateHomeSwitchDebounce();
    void enterHomingState(HomingState state);
    void failHoming();
    uint32_t homeSpeedSteps(float unitsPerSecond) const;
    uint32_t homeAccelerationSteps() const;
    long homeDistanceSteps(float units) const;
    void sanitizeHomeConfig();

    // Set M0, M1, M2 pins state
    void setMicrostepPins(int microsteps);
};

#endif // MOTOR_CONTROLLER_H
