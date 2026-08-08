#include "StorageManager.h"

StorageManager::StorageManager() {}

bool StorageManager::begin() {
    return _prefs.begin("stepper", false);
}

bool StorageManager::loadWifiConfig(WifiConfig& config) {
    size_t size = _prefs.getBytes("wifi_conf", &config, sizeof(WifiConfig));
    if (size != sizeof(WifiConfig)) {
        // Defaults: empty
        memset(&config, 0, sizeof(WifiConfig));
        // Save defaults
        return saveWifiConfig(config);
    }
    return true;
}

bool StorageManager::saveWifiConfig(const WifiConfig& config) {
    size_t size = _prefs.putBytes("wifi_conf", &config, sizeof(WifiConfig));
    return (size == sizeof(WifiConfig));
}

bool StorageManager::loadMotorConfig(int id, MotorConfig& config) {
    const char* key = (id == 1) ? "m1_conf" : "m2_conf";
    size_t size = _prefs.getBytes(key, &config, sizeof(MotorConfig));
    if (size != sizeof(MotorConfig)) {
        // Load default values
        config.maxSpeed = 2000.0f;
        config.acceleration = 1000.0f;
        config.minPosition = 0;
        config.maxPosition = (id == 1) ? 50000L : 100000L;
        
        config.stepAngle = 1.8f;
        config.microsteps = 16;
        config.driveType = DRIVE_LEAD_SCREW;
        config.leadScrewPitch = 8.0f;
        config.beltPitch = 2.0f;
        config.pulleyTeeth = 20;
        config.unitType = UNIT_MM;
        
        // Save default config
        return saveMotorConfig(id, config);
    }
    return true;
}

bool StorageManager::saveMotorConfig(int id, const MotorConfig& config) {
    const char* key = (id == 1) ? "m1_conf" : "m2_conf";
    size_t size = _prefs.putBytes(key, &config, sizeof(MotorConfig));
    return (size == sizeof(MotorConfig));
}

bool StorageManager::loadSystemConfig(SystemConfig& config) {
    size_t size = _prefs.getBytes("sys_conf", &config, sizeof(SystemConfig));
    if (size != sizeof(SystemConfig)) {
        config.enableTimeoutActive = true;
        config.enableTimeoutDuration = 10;
        config.motor1LastPos = 0;
        config.motor2LastPos = 0;
        return saveSystemConfig(config);
    }
    return true;
}

bool StorageManager::saveSystemConfig(const SystemConfig& config) {
    size_t size = _prefs.putBytes("sys_conf", &config, sizeof(SystemConfig));
    return (size == sizeof(SystemConfig));
}

bool StorageManager::loadProgramText(String& programText) {
    programText = _prefs.getString("program_text", "");
    return true;
}

bool StorageManager::saveProgramText(const String& programText) {
    return _prefs.putString("program_text", programText) > 0 || programText.length() == 0;
}

bool StorageManager::loadProgramText2(String& programText) {
    programText = _prefs.getString("program_2", "");
    return true;
}

bool StorageManager::saveProgramText2(const String& programText) {
    return _prefs.putString("program_2", programText) > 0 || programText.length() == 0;
}

bool StorageManager::loadSelectedProgramSlot(uint8_t& programSlot) {
    programSlot = _prefs.getUChar("program_sel", 1);
    if (programSlot != 1 && programSlot != 2) programSlot = 1;
    return true;
}

bool StorageManager::saveSelectedProgramSlot(uint8_t programSlot) {
    if (programSlot != 1 && programSlot != 2) return false;
    return _prefs.putUChar("program_sel", programSlot) == sizeof(uint8_t);
}

bool StorageManager::loadResetProgramText(String& programText) {
    programText = _prefs.getString("reset_prog", "");
    return true;
}

bool StorageManager::saveResetProgramText(const String& programText) {
    return _prefs.putString("reset_prog", programText) > 0 || programText.length() == 0;
}

bool StorageManager::savePositions(long pos1, long pos2) {
    SystemConfig config;
    if (loadSystemConfig(config)) {
        config.motor1LastPos = pos1;
        config.motor2LastPos = pos2;
        return saveSystemConfig(config);
    }
    return false;
}

void StorageManager::clearAll() {
    _prefs.clear();
}
