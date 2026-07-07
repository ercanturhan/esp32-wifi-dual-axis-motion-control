#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Motor 1 Pins
#define M1_STEP_PIN    25
#define M1_DIR_PIN     26
#define M1_ENABLE_PIN  27
#define M1_HOME_PIN    32
#define M1_M0_PIN      14
#define M1_M1_PIN      13
#define M1_M2_PIN      4

// Motor 2 Pins
#define M2_STEP_PIN    18
#define M2_DIR_PIN     19
#define M2_ENABLE_PIN  21
#define M2_HOME_PIN    33
#define M2_M0_PIN      16
#define M2_M1_PIN      17
#define M2_M2_PIN      22

// Timed Relay Output Pins
#define RELAY1_PIN     23
#define RELAY2_PIN     5
#define RELAY_ACTIVE_HIGH true

// AP WiFi Defaults
#define AP_SSID "ESP32-MOTOR"
#define AP_PASS "12345678"

// Watchdog Timer
#define WDT_TIMEOUT_SECONDS 10

// Motor Drive Types
enum DriveType {
    DRIVE_LEAD_SCREW = 0,
    DRIVE_BELT_PULLEY = 1
};

// Unit Types
enum UnitType {
    UNIT_PULSES = 0,
    UNIT_MM = 1,
    UNIT_CM = 2,
    UNIT_DEGREE = 3
};

// Structure for Motor Configuration
struct MotorConfig {
    float maxSpeed;        // steps per second
    float acceleration;    // steps per second^2
    long minPosition;      // in steps
    long maxPosition;      // in steps
    
    // Position Conversion Parameters
    float stepAngle;       // e.g., 1.8 degrees for NEMA14
    int microsteps;        // e.g., 16 for DRV8825 1/16
    int driveType;         // 0: Lead Screw, 1: Belt/Pulley
    float leadScrewPitch;  // Lead screw pitch in mm
    float beltPitch;       // Belt pitch in mm
    int pulleyTeeth;       // Number of teeth on pulley
    int unitType;          // 0: pulses, 1: mm, 2: cm, 3: degree
};

// Structure for WiFi Config
struct WifiConfig {
    char ssid[33];
    char password[65];
};

// Structure for System Settings
struct SystemConfig {
    bool enableTimeoutActive;     // true if we automatically disable driver after inactivity
    uint32_t enableTimeoutDuration; // inactivity duration in seconds (default 10s)
    long motor1LastPos;
    long motor2LastPos;
};

#endif // CONFIG_H
