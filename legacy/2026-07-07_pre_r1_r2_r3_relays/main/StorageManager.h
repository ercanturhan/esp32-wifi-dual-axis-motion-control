#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Preferences.h>
#include "Config.h"

class StorageManager {
public:
    StorageManager();
    bool begin();
    
    // WiFi Config
    bool loadWifiConfig(WifiConfig& config);
    bool saveWifiConfig(const WifiConfig& config);
    
    // Motor Config (id is 1 or 2)
    bool loadMotorConfig(int id, MotorConfig& config);
    bool saveMotorConfig(int id, const MotorConfig& config);
    
    // System Config
    bool loadSystemConfig(SystemConfig& config);
    bool saveSystemConfig(const SystemConfig& config);

    // Save Motor Positions specifically
    bool savePositions(long pos1, long pos2);

    // Helper to clear preferences
    void clearAll();

private:
    Preferences _prefs;
};

#endif // STORAGE_MANAGER_H
