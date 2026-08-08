#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include "Config.h"
#include "StorageManager.h"
#include "MotorController.h"
#include "RelayController.h"
#include "index_html.h"

class WebServerManager {
public:
    WebServerManager(StorageManager& storage, MotorController& m1, MotorController& m2,
                     RelayController& r1, RelayController& r2,
                     RelayController& programR1, RelayController& programR2, RelayController& programR3);
    
    void begin();
    void update();
    
    // Broadcast telemetry to all connected WebSocket clients
    void broadcastTelemetry();
    
    // Queue a message to be logged on the UI console
    void queueLog(const String& message);

    // Starts the last saved Program tab text from a physical input button.
    void startSavedProgramFromButton();
    void startSavedResetProgramFromButton();

    // Updates live telemetry for the physical Program Start button.
    void setProgramStartButtonPressed(bool pressed);
    void setResetProgramButtonPressed(bool pressed);

private:
    enum ProgramCommandType {
        PROGRAM_CMD_NONE = 0,
        PROGRAM_CMD_MOVE,
        PROGRAM_CMD_SPEED,
        PROGRAM_CMD_HOME,
        PROGRAM_CMD_DELAY,
        PROGRAM_CMD_GEN_OUTPUT,
        PROGRAM_CMD_RELAY
    };

    struct ProgramCommand {
        ProgramCommandType type;
        int motor;
        float value;
        int relay;
        bool relayOn;
        uint32_t delayMs;
    };

    StorageManager& _storage;
    MotorController& _m1;
    MotorController& _m2;
    RelayController& _r1;
    RelayController& _r2;
    RelayController& _programR1;
    RelayController& _programR2;
    RelayController& _programR3;
    
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    
    unsigned long _lastBroadcastTime;
    String _pendingLog;
    bool _hasPendingLog;

    String _programText;
    String _savedProgramText;
    String _savedProgramText2;
    String _savedResetProgramText;
    uint8_t _selectedProgramSlot;
    String _activeProgramName;
    String _programStatus;
    String _programCurrentLineText;
    bool _programRunning;
    size_t _programIndex;
    int _programLineNumber;
    int _programWaitingMotor;
    bool _programDelayActive;
    unsigned long _programDelayStartedAt;
    uint32_t _programDelayMs;
    bool _programStartButtonPressed;
    uint32_t _programStartButtonPressCount;
    bool _resetProgramButtonPressed;
    uint32_t _resetProgramButtonPressCount;
    
    WifiConfig _wifiConfig;
    
    void setupRoutes();
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);
    void processJsonCommand(JsonDocument& doc, AsyncWebSocketClient *client);
    void startProgram(const String& programText, const String& programName, bool saveText, int programSlot = 0);
    bool saveProgramText(const String& programText, int programSlot);
    bool selectProgramSlot(int programSlot);
    const String& getSavedProgramText(int programSlot) const;
    bool saveResetProgramText(const String& programText);
    void stopProgram(bool stopOutputs);
    void updateProgram();
    bool readNextProgramLine(String& line, int& lineNumber);
    bool parseProgramLine(String line, ProgramCommand& command, String& error);
    void setProgramStatus(const String& status);
    
    // Telemetry generation
    String getTelemetryJson();
    String getConfigJson();
};

#endif // WEB_SERVER_MANAGER_H
