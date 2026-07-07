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
                     RelayController& r1, RelayController& r2);
    
    void begin();
    void update();
    
    // Broadcast telemetry to all connected WebSocket clients
    void broadcastTelemetry();
    
    // Queue a message to be logged on the UI console
    void queueLog(const String& message);

private:
    enum ProgramCommandType {
        PROGRAM_CMD_NONE = 0,
        PROGRAM_CMD_MOVE,
        PROGRAM_CMD_DELAY,
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
    
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    
    unsigned long _lastBroadcastTime;
    String _pendingLog;
    bool _hasPendingLog;

    String _programText;
    String _programStatus;
    String _programCurrentLineText;
    bool _programRunning;
    size_t _programIndex;
    int _programLineNumber;
    int _programWaitingMotor;
    bool _programDelayActive;
    unsigned long _programDelayStartedAt;
    uint32_t _programDelayMs;
    
    WifiConfig _wifiConfig;
    
    void setupRoutes();
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);
    void processJsonCommand(JsonDocument& doc, AsyncWebSocketClient *client);
    void startProgram(const String& programText);
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
