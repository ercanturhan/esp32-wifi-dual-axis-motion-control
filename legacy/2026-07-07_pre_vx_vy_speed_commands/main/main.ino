#include <Arduino.h>
#include <esp_task_wdt.h>
#include "Config.h"
#include "StorageManager.h"
#include "MotorController.h"
#include "RelayController.h"
#include "WebServerManager.h"

// Hardware Timer Engine for FastAccelStepper
FastAccelStepperEngine engine = FastAccelStepperEngine();

// Managers & Controllers
StorageManager storage;
MotorController motor1;
MotorController motor2;
RelayController relay1;
RelayController relay2;
WebServerManager webServer(storage, motor1, motor2, relay1, relay2);

// Configuration copies
MotorConfig m1Cfg;
MotorConfig m2Cfg;
SystemConfig sysCfg;

// Dynamic NVS positions saving tracker
unsigned long lastMoveTime = 0;
long lastSavedPos1 = 0;
long lastSavedPos2 = 0;

void setup() {
    // 1. Initialize Serial
    Serial.begin(115200);
    while(!Serial && millis() < 2000); // Wait up to 2s for native USB
    Serial.println("\n=============================================");
    Serial.println("ESP32 Dual Axis Stepper Motor System Initializing...");
    Serial.println("=============================================");

    // 2. Initialize NVS Storage
    if (!storage.begin()) {
        Serial.println("ERROR: Storage initialization failed! NVS Preferences error.");
    } else {
        Serial.println("Storage Manager initialized.");
    }

    // 3. Load Configurations
    storage.loadMotorConfig(1, m1Cfg);
    storage.loadMotorConfig(2, m2Cfg);
    storage.loadSystemConfig(sysCfg);
    
    lastSavedPos1 = sysCfg.motor1LastPos;
    lastSavedPos2 = sysCfg.motor2LastPos;
    
    Serial.printf("Motor 1 Loaded Pos: %ld, Min Limit: %ld, Max Limit: %ld\n", m1Cfg.minPosition, m1Cfg.maxPosition, lastSavedPos1);
    Serial.printf("Motor 2 Loaded Pos: %ld, Min Limit: %ld, Max Limit: %ld\n", m2Cfg.minPosition, m2Cfg.maxPosition, lastSavedPos2);

    // 4. Initialize FastAccelStepper Engine
    engine.init();
    Serial.println("FastAccelStepper engine initialized.");

    // 5. Initialize Motor Controllers
    motor1.begin(1, engine, m1Cfg, M1_STEP_PIN, M1_DIR_PIN, M1_ENABLE_PIN, M1_HOME_PIN, M1_M0_PIN, M1_M1_PIN, M1_M2_PIN);
    motor2.begin(2, engine, m2Cfg, M2_STEP_PIN, M2_DIR_PIN, M2_ENABLE_PIN, M2_HOME_PIN, M2_M0_PIN, M2_M1_PIN, M2_M2_PIN);
    
    // Restore positions
    motor1.setCurrentPosition(lastSavedPos1);
    motor2.setCurrentPosition(lastSavedPos2);
    
    // Configure timeouts
    motor1.setEnableTimeout(sysCfg.enableTimeoutActive, sysCfg.enableTimeoutDuration);
    motor2.setEnableTimeout(sysCfg.enableTimeoutActive, sysCfg.enableTimeoutDuration);
    Serial.println("Motor controllers initialized and coordinates restored.");

    // 6. Initialize timed relay outputs
    relay1.begin(1, RELAY1_PIN, RELAY_ACTIVE_HIGH);
    relay2.begin(2, RELAY2_PIN, RELAY_ACTIVE_HIGH);
    Serial.printf("Relay 1 initialized on GPIO%d.\n", RELAY1_PIN);
    Serial.printf("Relay 2 initialized on GPIO%d.\n", RELAY2_PIN);

    // 7. Initialize Web server and AP+STA Wi-Fi
    webServer.begin();
    webServer.queueLog("System loaded successfully.");

    // 8. Initialize Task Watchdog (10 seconds timeout)
    Serial.println("Configuring Task Watchdog...");
    #if ESP_IDF_VERSION_MAJOR >= 5
        esp_task_wdt_config_t wdt_config;
        wdt_config.timeout_ms = WDT_TIMEOUT_SECONDS * 1000;
        wdt_config.idle_core_mask = 0;
        wdt_config.trigger_panic = true;
        esp_task_wdt_init(&wdt_config);
    #else
        esp_task_wdt_init(WDT_TIMEOUT_SECONDS, true);
    #endif
    esp_task_wdt_add(NULL); // Add loop task
    Serial.println("Watchdog initialized successfully.");
}

void loop() {
    // 1. Feed the Watchdog Timer
    esp_task_wdt_reset();

    // 2. Update Motor State Machines (Homing & Auto-Enable timeouts)
    motor1.update();
    motor2.update();

    // 3. Update timed relay outputs
    relay1.update();
    relay2.update();

    // 4. Update Web Server events & WebSocket Broadcasts
    webServer.update();

    // 5. Smart NVS Position Auto-Save
    long currPos1 = motor1.getCurrentPosition();
    long currPos2 = motor2.getCurrentPosition();

    if (motor1.isRunning() || motor2.isRunning() || motor1.isHoming() || motor2.isHoming()) {
        lastMoveTime = millis();
    } else {
        // If both motors are idle and positions have changed
        if (currPos1 != lastSavedPos1 || currPos2 != lastSavedPos2) {
            // Save after 2 seconds of inactivity to protect NVS write wear
            if (millis() - lastMoveTime > 2000) {
                storage.savePositions(currPos1, currPos2);
                lastSavedPos1 = currPos1;
                lastSavedPos2 = currPos2;
                webServer.queueLog("Coordinates saved to flash memory.");
            }
        }
    }
}
