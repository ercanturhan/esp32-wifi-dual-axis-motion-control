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

    // Sequential program text used by the web Program tab and physical start button
    bool loadProgramText(String& programText);
    bool saveProgramText(const String& programText);
    bool loadProgramText2(String& programText);
    bool saveProgramText2(const String& programText);
    bool loadSelectedProgramSlot(uint8_t& programSlot);
    bool saveSelectedProgramSlot(uint8_t programSlot);

    // Separate reset program text used by the Reset Program editor and physical reset button
    bool loadResetProgramText(String& programText);
    bool saveResetProgramText(const String& programText);

    // Save Motor Positions specifically
    bool savePositions(long pos1, long pos2);

    // Helper to clear preferences
    void clearAll();

private:
    Preferences _prefs;
};

#endif // STORAGE_MANAGER_H
