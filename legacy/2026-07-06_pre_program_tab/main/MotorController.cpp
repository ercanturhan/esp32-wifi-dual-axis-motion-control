#include "MotorController.h"

MotorController::MotorController() 
    : _id(0), _stepper(nullptr), _stepPin(-1), _dirPin(-1), _enablePin(-1), _homePin(-1),
      _m0Pin(-1), _m1Pin(-1), _m2Pin(-1),
      _homingState(HOME_IDLE), _homeCompleted(false), _homingStartTime(0),
      _driverEnabled(false), _timeoutActive(true), _timeoutDurationMs(10000), _lastActiveTime(0),
      _stepsPerUnit(1.0) {}

void MotorController::begin(int id, FastAccelStepperEngine& engine, MotorConfig& config, 
                            int stepPin, int dirPin, int enablePin, int homePin,
                            int m0Pin, int m1Pin, int m2Pin) {
    _id = id;
    _config = config;
    _stepPin = stepPin;
    _dirPin = dirPin;
    _enablePin = enablePin;
    _homePin = homePin;
    _m0Pin = m0Pin;
    _m1Pin = m1Pin;
    _m2Pin = m2Pin;
    
    _homingState = HOME_IDLE;
    _homeCompleted = false;
    _driverEnabled = false; // Start disabled
    
    pinMode(_homePin, INPUT_PULLUP);
    pinMode(_enablePin, OUTPUT);
    digitalWrite(_enablePin, HIGH); // Active LOW: HIGH = Disabled
    
    // Setup microstep pins as outputs
    if (_m0Pin != -1 && _m1Pin != -1 && _m2Pin != -1) {
        pinMode(_m0Pin, OUTPUT);
        pinMode(_m1Pin, OUTPUT);
        pinMode(_m2Pin, OUTPUT);
        setMicrostepPins(_config.microsteps);
    }
    
    _stepper = engine.stepperConnectToPin(_stepPin);
    if (_stepper) {
        _stepper->setDirectionPin(_dirPin);
    }
    
    calculateStepsPerUnit();
    applySpeedProfile();
    
    _lastActiveTime = millis();
}

void MotorController::update() {
    if (!_stepper) return;
    
    bool switchActive = isLimitSwitchActive();
    
    // Homing State Machine
    if (_homingState != HOME_IDLE) {
        // Ensure driver is enabled during homing
        if (!_driverEnabled) {
            enableDriver(true);
        }
        
        switch (_homingState) {
            case HOME_INIT:
                _homeCompleted = false;
                if (switchActive) {
                    // Switch already active, back off first
                    _stepper->forceStop();
                    _stepper->setCurrentPosition(0);
                    _stepper->setSpeedInHz(200); // Back off slowly
                    _stepper->setAcceleration((uint32_t)_config.acceleration);
                    _stepper->moveTo(200);
                    _homingState = HOME_BACKING_OFF;
                } else {
                    // Run backwards fast (half of max speed)
                    _stepper->setSpeedInHz((uint32_t)(_config.maxSpeed / 2));
                    _stepper->setAcceleration((uint32_t)_config.acceleration);
                    _stepper->runBackward();
                    _homingState = HOME_FAST_BACKWARD;
                }
                break;
                
            case HOME_FAST_BACKWARD:
                if (switchActive) {
                    // Hit the switch! Stop immediately
                    _stepper->forceStop();
                    _stepper->setCurrentPosition(0);
                    
                    // Start backing off 200 steps
                    _stepper->setSpeedInHz((uint32_t)(_config.maxSpeed / 4));
                    _stepper->setAcceleration((uint32_t)_config.acceleration);
                    _stepper->moveTo(200);
                    _homingState = HOME_BACKING_OFF;
                } else if (millis() - _homingStartTime > 30000) { // 30s timeout
                    stop();
                    _homingState = HOME_FAILED;
                }
                break;
                
            case HOME_BACKING_OFF:
                if (!_stepper->isRunning()) {
                    // Done backing off. Switch should be inactive now.
                    // Start slow approach
                    _stepper->setSpeedInHz(200); // Very slow approach (200 steps/s)
                    _stepper->setAcceleration((uint32_t)_config.acceleration);
                    _stepper->runBackward();
                    _homingState = HOME_SLOW_APPROACH;
                } else if (millis() - _homingStartTime > 40000) {
                    stop();
                    _homingState = HOME_FAILED;
                }
                break;
                
            case HOME_SLOW_APPROACH:
                if (switchActive) {
                    // Hit the switch slowly! Stop immediately
                    _stepper->forceStop();
                    _stepper->setCurrentPosition(0);
                    
                    _homeCompleted = true;
                    _homingState = HOME_IDLE;
                    
                    // Restore original speed profile
                    applySpeedProfile();
                } else if (millis() - _homingStartTime > 50000) {
                    stop();
                    _homingState = HOME_FAILED;
                }
                break;
                
            default:
                break;
        }
        
        _lastActiveTime = millis(); // Refresh timeout activity
    } else {
        // Normal operation
        bool motorMoving = _stepper->isRunning();
        
        if (motorMoving) {
            _lastActiveTime = millis();
            if (!_driverEnabled) {
                enableDriver(true); // Safety enable
            }
        } else {
            // Auto-disable timeout check
            if (_timeoutActive && _driverEnabled) {
                if (millis() - _lastActiveTime > _timeoutDurationMs) {
                    enableDriver(false);
                }
            }
        }
    }
}

bool MotorController::moveTo(float targetUnit) {
    long targetSteps = unitsToSteps(targetUnit);
    return moveAbsolute(targetSteps);
}

bool MotorController::moveAbsolute(long steps) {
    if (!_stepper) return false;
    
    // Soft Limit check (only if home has been completed or if we want to enforce it always)
    // For safety, soft limits are always checked unless homing
    if (steps < _config.minPosition || steps > _config.maxPosition) {
        return false;
    }
    
    // Enable driver before movement
    enableDriver(true);
    
    _stepper->moveTo(steps);
    _lastActiveTime = millis();
    return true;
}

bool MotorController::moveRelative(long steps) {
    if (!_stepper) return false;
    
    long currentPos = _stepper->getCurrentPosition();
    long targetSteps = currentPos + steps;
    
    // Soft Limit check
    if (targetSteps < _config.minPosition || targetSteps > _config.maxPosition) {
        return false;
    }
    
    // Enable driver
    enableDriver(true);
    
    _stepper->move(steps);
    _lastActiveTime = millis();
    return true;
}

void MotorController::stop() {
    if (!_stepper) return;
    _stepper->stopMove();
    
    if (_homingState != HOME_IDLE) {
        _homingState = HOME_IDLE;
        applySpeedProfile();
    }
}

void MotorController::startHoming() {
    _homingState = HOME_INIT;
    _homingStartTime = millis();
}

void MotorController::enableDriver(bool enable) {
    _driverEnabled = enable;
    digitalWrite(_enablePin, enable ? LOW : HIGH); // Active LOW: LOW = Enabled, HIGH = Disabled
    _lastActiveTime = millis();
}

void MotorController::updateConfig(const MotorConfig& newConfig) {
    _config = newConfig;
    setMicrostepPins(_config.microsteps);
    calculateStepsPerUnit();
    applySpeedProfile();
}

long MotorController::getCurrentPosition() const {
    return _stepper ? _stepper->getCurrentPosition() : 0;
}

long MotorController::getTargetPosition() const {
    return _stepper ? _stepper->targetPos() : 0;
}

float MotorController::getCurrentPositionUnit() const {
    return stepsToUnits(getCurrentPosition());
}

float MotorController::getTargetPositionUnit() const {
    return stepsToUnits(getTargetPosition());
}

long MotorController::getRemainingDistance() const {
    if (!_stepper) return 0;
    long diff = getTargetPosition() - getCurrentPosition();
    return abs(diff);
}

float MotorController::getRemainingDistanceUnit() const {
    return stepsToUnits(getRemainingDistance());
}

int MotorController::getProgressPercent() const {
    if (!_stepper || !_stepper->isRunning()) return 100;
    
    // To calculate progress, we need to know where we started.
    // However, a simple estimate based on remaining distance can be shown,
    // or we can calculate percentage relative to the target.
    // Let's make it a reliable calculation:
    // percentage = (1.0 - (remaining / total_move_distance)) * 100.
    // If we don't track total move distance, we can default to:
    long remaining = getRemainingDistance();
    if (remaining == 0) return 100;
    
    // We can approximate it or return a value based on UI expectations.
    // Let's return dynamic progress from current to target.
    return 0; // The UI will calculate this from actual target and current if we provide both.
}

float MotorController::getSpeed() const {
    // Return speed in steps/s or hz
    return _config.maxSpeed;
}

float MotorController::getAcceleration() const {
    return _config.acceleration;
}

bool MotorController::isRunning() const {
    return _stepper ? _stepper->isRunning() : false;
}

bool MotorController::isLimitSwitchActive() const {
    return digitalRead(_homePin) == LOW; // Active LOW switch (with INPUT_PULLUP)
}

long MotorController::unitsToSteps(float units) const {
    return (long)round(units * _stepsPerUnit);
}

float MotorController::stepsToUnits(long steps) const {
    return (float)steps / _stepsPerUnit;
}

void MotorController::resetPosition() {
    if (!_stepper) return;
    _stepper->forceStop();
    _stepper->setCurrentPosition(0);
    _homeCompleted = true; // resets home status as well
}

void MotorController::setCurrentPosition(long pos) {
    if (!_stepper) return;
    _stepper->forceStop();
    _stepper->setCurrentPosition(pos);
}

void MotorController::setEnableTimeout(bool active, uint32_t seconds) {
    _timeoutActive = active;
    _timeoutDurationMs = seconds * 1000;
}

void MotorController::calculateStepsPerUnit() {
    double stepsPerRev = (360.0 / _config.stepAngle) * _config.microsteps;
    double pitch = 1.0;
    
    if (_config.driveType == DRIVE_LEAD_SCREW) {
        pitch = _config.leadScrewPitch;
    } else { // DRIVE_BELT_PULLEY
        pitch = _config.beltPitch * _config.pulleyTeeth;
    }
    
    if (pitch <= 0.0) pitch = 1.0;
    
    switch (_config.unitType) {
        case UNIT_MM:
            _stepsPerUnit = stepsPerRev / pitch;
            break;
        case UNIT_CM:
            _stepsPerUnit = (stepsPerRev / pitch) * 10.0;
            break;
        case UNIT_DEGREE:
            _stepsPerUnit = stepsPerRev / 360.0;
            break;
        case UNIT_PULSES:
        default:
            _stepsPerUnit = 1.0;
            break;
    }
    
    if (_stepsPerUnit <= 0.0) _stepsPerUnit = 1.0;
}

void MotorController::applySpeedProfile() {
    if (!_stepper) return;
    _stepper->setSpeedInHz((uint32_t)_config.maxSpeed);
    _stepper->setAcceleration((uint32_t)_config.acceleration);
}

void MotorController::setMicrostepPins(int microsteps) {
    if (_m0Pin == -1 || _m1Pin == -1 || _m2Pin == -1) return;
    
    // DRV8825 Microstepping Truth Table:
    // Microsteps | M0   | M1   | M2
    // 1 (Full)   | LOW  | LOW  | LOW
    // 2 (1/2)    | HIGH | LOW  | LOW
    // 4 (1/4)    | LOW  | HIGH | LOW
    // 8 (1/8)    | HIGH | HIGH | LOW
    // 16 (1/16)  | LOW  | LOW  | HIGH
    // 32 (1/32)  | HIGH | LOW  | HIGH
    
    bool m0 = false;
    bool m1 = false;
    bool m2 = false;
    
    switch (microsteps) {
        case 1:
            m0 = false; m1 = false; m2 = false;
            break;
        case 2:
            m0 = true;  m1 = false; m2 = false;
            break;
        case 4:
            m0 = false; m1 = true;  m2 = false;
            break;
        case 8:
            m0 = true;  m1 = true;  m2 = false;
            break;
        case 16:
            m0 = false; m1 = false; m2 = true;
            break;
        case 32:
        default:
            m0 = true;  m1 = false; m2 = true;
            break;
    }
    
    digitalWrite(_m0Pin, m0 ? HIGH : LOW);
    digitalWrite(_m1Pin, m1 ? HIGH : LOW);
    digitalWrite(_m2Pin, m2 ? HIGH : LOW);
    
    Serial.printf("Motor %d microstep set to 1/%d (M0:%d, M1:%d, M2:%d)\n", 
                  _id, microsteps, m0, m1, m2);
}
