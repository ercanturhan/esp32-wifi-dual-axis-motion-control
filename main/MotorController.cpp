#include "MotorController.h"

MotorController::MotorController() 
    : _id(0), _stepper(nullptr), _stepPin(-1), _dirPin(-1), _enablePin(-1), _homePin(-1),
      _m0Pin(-1), _m1Pin(-1), _m2Pin(-1),
      _homingState(HOME_IDLE), _homeCompleted(false), _homingStartTime(0), _homingStateStartTime(0),
      _homeSwitchRaw(false), _homeSwitchStable(false), _homeSwitchChangedAt(0),
      _driverEnabled(false), _timeoutActive(true), _timeoutDurationMs(10000), _lastActiveTime(0),
      _lastMoveBlockedByHomeSwitch(false), _homeLimitEventPending(false),
      _plannedTarget(0), _stepsPerUnit(1.0) {}

void MotorController::begin(int id, FastAccelStepperEngine& engine, MotorConfig& config, 
                            int stepPin, int dirPin, int enablePin, int homePin,
                            int m0Pin, int m1Pin, int m2Pin) {
    _id = id;
    _config = config;
    sanitizeHomeConfig();
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
    _lastMoveBlockedByHomeSwitch = false;
    _homeLimitEventPending = false;
    _plannedTarget = 0;
    
    pinMode(_homePin, INPUT_PULLUP);
    _homeSwitchRaw = digitalRead(_homePin) == LOW;
    _homeSwitchStable = _homeSwitchRaw;
    _homeSwitchChangedAt = millis();
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

    updateHomeSwitchDebounce();
    bool switchActive = _homeSwitchRaw;
    
    // Homing State Machine
    if (isHoming()) {
        // Ensure driver is enabled during homing
        if (!_driverEnabled) {
            enableDriver(true);
        }
        
        switch (_homingState) {
            case HOME_INIT:
                _homeCompleted = false;
                if (millis() - _homingStateStartTime < _config.homeDirectionSettleMs) {
                    break;
                }
                if (_homeSwitchStable) {
                    // Discard any queued motion before reversing away from an
                    // already active switch.
                    _stepper->forceStopAndNewPosition(0);
                    enterHomingState(HOME_BACKOFF_SETTLE);
                } else if (!switchActive) {
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeFastSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runBackward();
                    enterHomingState(HOME_FAST_BACKWARD);
                }
                break;
                
            case HOME_FAST_BACKWARD:
                if (switchActive) {
                    // Stop on the first raw LOW sample. Confirm the switch only
                    // after motion has stopped, so debounce never adds overtravel.
                    _stepper->forceStopAndNewPosition(0);
                    enterHomingState(HOME_FAST_SWITCH_CONFIRM);
                } else if (millis() - _homingStateStartTime > _config.homeSearchTimeoutMs) {
                    failHoming();
                }
                break;

            case HOME_FAST_SWITCH_CONFIRM:
                if (_homeSwitchStable) {
                    enterHomingState(HOME_BACKOFF_SETTLE);
                } else if (!switchActive && millis() - _homingStateStartTime >= _config.homeSwitchDebounceMs) {
                    // Electrical noise only: resume the fast search in the
                    // known home direction from a completely empty queue.
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeFastSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runBackward();
                    enterHomingState(HOME_FAST_BACKWARD);
                }
                break;

            case HOME_BACKOFF_SETTLE:
                if (millis() - _homingStateStartTime >= _config.homeDirectionSettleMs) {
                    // Direction changes only after the previous queue is empty
                    // and the DRV8825 DIR input has had ample setup time.
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeFastSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runForward();
                    enterHomingState(HOME_BACKING_OFF);
                }
                break;
                
            case HOME_BACKING_OFF:
                if (!_homeSwitchStable) {
                    // Continue in the same direction without stopping. Once the
                    // switch is stably released, add a physical clearance distance.
                    long clearanceTarget = _stepper->getCurrentPosition() +
                                           homeDistanceSteps(_config.homeBackoffDistance);
                    _stepper->moveTo(clearanceTarget);
                    enterHomingState(HOME_BACKOFF_CLEARANCE);
                } else if (_stepper->getCurrentPosition() >= homeDistanceSteps(_config.homeMaxReleaseDistance)) {
                    failHoming();
                } else if (millis() - _homingStateStartTime > _config.homeSearchTimeoutMs) {
                    failHoming();
                }
                break;

            case HOME_BACKOFF_CLEARANCE:
                if (!_stepper->isRunning()) {
                    if (_homeSwitchStable) {
                        // The contact re-engaged or bounced. Keep moving away in
                        // one continuous pass until it is stably released again.
                        _stepper->runForward();
                        enterHomingState(HOME_BACKING_OFF);
                    } else {
                        enterHomingState(HOME_SLOW_APPROACH_SETTLE);
                    }
                }
                break;

            case HOME_SLOW_APPROACH_SETTLE:
                if (_homeSwitchStable) {
                    // A bouncing/re-engaged switch must be cleared before the
                    // precision approach can reverse toward home.
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeFastSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runForward();
                    enterHomingState(HOME_BACKING_OFF);
                } else if (!switchActive && millis() - _homingStateStartTime >= _config.homeDirectionSettleMs) {
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeSlowSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runBackward();
                    enterHomingState(HOME_SLOW_APPROACH);
                }
                break;
                
            case HOME_SLOW_APPROACH:
                if (switchActive) {
                    // Stop immediately, then validate the contact while stationary.
                    _stepper->forceStopAndNewPosition(0);
                    enterHomingState(HOME_SLOW_SWITCH_CONFIRM);
                } else if (millis() - _homingStateStartTime > _config.homeSearchTimeoutMs) {
                    failHoming();
                }
                break;

            case HOME_SLOW_SWITCH_CONFIRM:
                if (_homeSwitchStable) {
                    // Homing owns both the physical coordinate and the
                    // controller's synchronous target. Leaving the old manual
                    // target here made the first post-home relative move start
                    // from that stale value instead of zero.
                    _stepper->forceStopAndNewPosition(0);
                    _plannedTarget = 0;
                    _lastMoveBlockedByHomeSwitch = false;
                    _homeLimitEventPending = false;
                    _homeCompleted = true;
                    _homingState = HOME_IDLE;
                    applySpeedProfile();
                } else if (!switchActive && millis() - _homingStateStartTime >= _config.homeSwitchDebounceMs) {
                    _stepper->setSpeedInHz(homeSpeedSteps(_config.homeSlowSpeed));
                    _stepper->setAcceleration(homeAccelerationSteps());
                    _stepper->runBackward();
                    enterHomingState(HOME_SLOW_APPROACH);
                }
                break;
                
            default:
                break;
        }
        
        _lastActiveTime = millis(); // Refresh timeout activity
    } else {
        // Normal operation
        bool motorMoving = _stepper->isRunning();

        // Homing uses the negative direction. During every other move, stop immediately
        // if the home switch becomes active while the target is further negative.
        if (motorMoving && switchActive && _plannedTarget < _stepper->getCurrentPosition()) {
            long stopPosition = _stepper->getCurrentPosition();
            _stepper->forceStopAndNewPosition(stopPosition);
            _plannedTarget = stopPosition;
            _lastMoveBlockedByHomeSwitch = true;
            _homeLimitEventPending = true;
            _lastActiveTime = millis();
            return;
        }
        
        if (motorMoving) {
            _lastActiveTime = millis();
            if (!_driverEnabled) {
                enableDriver(true); // Safety enable
            }
        } else {
            _plannedTarget = _stepper->getCurrentPosition();
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

    _lastMoveBlockedByHomeSwitch = false;
    long currentPos = _stepper->getCurrentPosition();
    if (isLimitSwitchActive() && steps < currentPos) {
        _lastMoveBlockedByHomeSwitch = true;
        _homeLimitEventPending = true;
        return false;
    }
    
    // Soft Limit check (only if home has been completed or if we want to enforce it always)
    // For safety, soft limits are always checked unless homing
    if (steps < _config.minPosition || steps > _config.maxPosition) {
        return false;
    }

    // FastAccelStepper silently ignores a target that reverses an active move.
    // Flush the old queue so the requested direction takes effect immediately.
    bool hasActivePlan = _stepper->isRunning() || _plannedTarget != currentPos;
    if (hasActivePlan) {
        long activeDelta = _plannedTarget - currentPos;
        long requestedDelta = steps - currentPos;
        if ((activeDelta > 0 && requestedDelta < 0) ||
            (activeDelta < 0 && requestedDelta > 0)) {
            _stepper->forceStopAndNewPosition(currentPos);
            _plannedTarget = currentPos;
        }
    }

    // Enable driver before movement
    enableDriver(true);

    _plannedTarget = steps;
    if (_stepper->moveTo(steps) != MOVE_OK) {
        _plannedTarget = _stepper->getCurrentPosition();
        return false;
    }
    _lastActiveTime = millis();
    return true;
}

bool MotorController::moveRelative(long steps, bool clampToLimits) {
    if (!_stepper) return false;

    _lastMoveBlockedByHomeSwitch = false;
    if (isLimitSwitchActive() && steps < 0) {
        _lastMoveBlockedByHomeSwitch = true;
        _homeLimitEventPending = true;
        return false;
    }
    
    long currentPos = _stepper->getCurrentPosition();
    bool running = _stepper->isRunning();
    bool hasActivePlan = running || _plannedTarget != currentPos;
    long baseTarget = hasActivePlan ? _plannedTarget : currentPos;

    // An opposite jog replaces the active queue. Without this flush the
    // library silently ignores the direction reversal until the old move ends.
    long activeDelta = _plannedTarget - currentPos;
    if (hasActivePlan && ((activeDelta > 0 && steps < 0) ||
                          (activeDelta < 0 && steps > 0))) {
        _stepper->forceStopAndNewPosition(currentPos);
        _plannedTarget = currentPos;
        baseTarget = currentPos;
    }

    // Rapid presses accumulate against the last accepted target rather than
    // the lagging physical position. Manual jogs clamp at the limit; program
    // moves remain strict and report a soft-limit violation.
    int64_t requestedTarget = (int64_t)baseTarget + (int64_t)steps;
    if (requestedTarget < _config.minPosition || requestedTarget > _config.maxPosition) {
        if (!clampToLimits) return false;
        if (requestedTarget < _config.minPosition) requestedTarget = _config.minPosition;
        if (requestedTarget > _config.maxPosition) requestedTarget = _config.maxPosition;
    }
    long targetSteps = (long)requestedTarget;

    if (targetSteps == baseTarget) {
        _plannedTarget = baseTarget;
        return true;
    }

    // Enable driver
    enableDriver(true);

    _plannedTarget = targetSteps;
    if (_stepper->moveTo(targetSteps) != MOVE_OK) {
        _plannedTarget = _stepper->getCurrentPosition();
        return false;
    }
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
    if (!_stepper) return;

    // A home request is a new controlled operation. Cancel any queued manual
    // or program move first so its previous direction cannot leak into homing.
    long currentPosition = _stepper->getCurrentPosition();
    _stepper->forceStopAndNewPosition(currentPosition);
    _plannedTarget = currentPosition;
    _lastMoveBlockedByHomeSwitch = false;
    _homeLimitEventPending = false;
    _homeCompleted = false;
    _homingStartTime = millis();
    enterHomingState(HOME_INIT);
}

void MotorController::enableDriver(bool enable) {
    _driverEnabled = enable;
    digitalWrite(_enablePin, enable ? LOW : HIGH); // Active LOW: LOW = Enabled, HIGH = Disabled
    _lastActiveTime = millis();
}

void MotorController::updateConfig(const MotorConfig& newConfig) {
    _config = newConfig;
    sanitizeHomeConfig();
    setMicrostepPins(_config.microsteps);
    calculateStepsPerUnit();
    applySpeedProfile();
}

bool MotorController::setRuntimeSpeed(float speedStepsPerSecond) {
    if (!_stepper) return false;
    if (speedStepsPerSecond < 1.0f || speedStepsPerSecond > 200000.0f) return false;

    _config.maxSpeed = speedStepsPerSecond;
    _stepper->setSpeedInHz((uint32_t)round(speedStepsPerSecond));
    return true;
}

long MotorController::getCurrentPosition() const {
    return _stepper ? _stepper->getCurrentPosition() : 0;
}

long MotorController::getTargetPosition() const {
    if (!_stepper) return 0;
    return isHoming() ? _stepper->targetPos() : _plannedTarget;
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

bool MotorController::isHoming() const {
    return _homingState != HOME_IDLE && _homingState != HOME_FAILED;
}

bool MotorController::isLimitSwitchActive() const {
    return digitalRead(_homePin) == LOW; // Active LOW switch (with INPUT_PULLUP)
}

void MotorController::updateHomeSwitchDebounce() {
    bool raw = digitalRead(_homePin) == LOW;
    if (raw != _homeSwitchRaw) {
        _homeSwitchRaw = raw;
        _homeSwitchChangedAt = millis();
    }

    if (_homeSwitchStable != _homeSwitchRaw &&
        millis() - _homeSwitchChangedAt >= _config.homeSwitchDebounceMs) {
        _homeSwitchStable = _homeSwitchRaw;
    }
}

void MotorController::enterHomingState(HomingState state) {
    _homingState = state;
    _homingStateStartTime = millis();
}

void MotorController::failHoming() {
    if (_stepper) {
        long currentPosition = _stepper->getCurrentPosition();
        _stepper->forceStopAndNewPosition(currentPosition);
        _plannedTarget = currentPosition;
    }
    _homeCompleted = false;
    _homingState = HOME_FAILED;
    applySpeedProfile();
}

uint32_t MotorController::homeSpeedSteps(float unitsPerSecond) const {
    double stepsPerSecond = fabs((double)unitsPerSecond) * _stepsPerUnit;
    if (stepsPerSecond < 1.0) stepsPerSecond = 1.0;
    if (stepsPerSecond > 200000.0) stepsPerSecond = 200000.0;
    return (uint32_t)round(stepsPerSecond);
}

uint32_t MotorController::homeAccelerationSteps() const {
    double stepsPerSecondSquared = fabs((double)_config.homeAcceleration) * _stepsPerUnit;
    if (stepsPerSecondSquared < 1.0) stepsPerSecondSquared = 1.0;
    if (stepsPerSecondSquared > 200000.0) stepsPerSecondSquared = 200000.0;
    return (uint32_t)round(stepsPerSecondSquared);
}

long MotorController::homeDistanceSteps(float units) const {
    double steps = fabs((double)units) * _stepsPerUnit;
    if (steps < 1.0) steps = 1.0;
    if (steps > 2000000000.0) steps = 2000000000.0;
    return (long)round(steps);
}

void MotorController::sanitizeHomeConfig() {
    if (!isfinite(_config.homeFastSpeed) || _config.homeFastSpeed <= 0.0f) _config.homeFastSpeed = 4.0f;
    if (!isfinite(_config.homeSlowSpeed) || _config.homeSlowSpeed <= 0.0f) _config.homeSlowSpeed = 1.5f;
    if (!isfinite(_config.homeAcceleration) || _config.homeAcceleration <= 0.0f) _config.homeAcceleration = 3.0f;
    if (!isfinite(_config.homeBackoffDistance) || _config.homeBackoffDistance <= 0.0f) _config.homeBackoffDistance = 1.0f;
    if (!isfinite(_config.homeMaxReleaseDistance) ||
        _config.homeMaxReleaseDistance < _config.homeBackoffDistance) {
        _config.homeMaxReleaseDistance = max(5.0f, _config.homeBackoffDistance);
    }
    _config.homeDirectionSettleMs = constrain(_config.homeDirectionSettleMs, 10UL, 2000UL);
    _config.homeSwitchDebounceMs = constrain(_config.homeSwitchDebounceMs, 5UL, 500UL);
    _config.homeSearchTimeoutMs = constrain(_config.homeSearchTimeoutMs, 1000UL, 120000UL);
}

bool MotorController::consumeHomeLimitEvent() {
    bool pending = _homeLimitEventPending;
    _homeLimitEventPending = false;
    return pending;
}

long MotorController::unitsToSteps(float units) const {
    return (long)round(units * _stepsPerUnit);
}

float MotorController::stepsToUnits(long steps) const {
    return (float)steps / _stepsPerUnit;
}

void MotorController::resetPosition() {
    if (!_stepper) return;
    _stepper->forceStopAndNewPosition(0);
    _plannedTarget = 0;
    _homeCompleted = true; // resets home status as well
}

void MotorController::setCurrentPosition(long pos) {
    if (!_stepper) return;
    _stepper->forceStopAndNewPosition(pos);
    _plannedTarget = pos;
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
