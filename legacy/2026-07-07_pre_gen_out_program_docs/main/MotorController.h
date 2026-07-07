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
    HOME_BACKING_OFF,
    HOME_SLOW_APPROACH,
    HOME_COMPLETED,
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
    bool moveRelative(long steps);
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
    bool isHoming() const { return _homingState != HOME_IDLE; }
    bool isHomeCompleted() const { return _homeCompleted; }
    bool isLimitSwitchActive() const;
    
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
    
    // Driver Enable / Timeout Management
    bool _driverEnabled;
    bool _timeoutActive;
    uint32_t _timeoutDurationMs;
    unsigned long _lastActiveTime;
    
    // Conversion factor
    double _stepsPerUnit;
    void calculateStepsPerUnit();
    
    // Apply speed/accel from config
    void applySpeedProfile();

    // Set M0, M1, M2 pins state
    void setMicrostepPins(int microsteps);
};

#endif // MOTOR_CONTROLLER_H
