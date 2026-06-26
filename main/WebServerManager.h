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
#include "index_html.h"

class WebServerManager {
public:
    WebServerManager(StorageManager& storage, MotorController& m1, MotorController& m2);
    
    void begin();
    void update();
    
    // Broadcast telemetry to all connected WebSocket clients
    void broadcastTelemetry();
    
    // Queue a message to be logged on the UI console
    void queueLog(const String& message);

private:
    StorageManager& _storage;
    MotorController& _m1;
    MotorController& _m2;
    
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    
    unsigned long _lastBroadcastTime;
    String _pendingLog;
    bool _hasPendingLog;
    
    WifiConfig _wifiConfig;
    
    void setupRoutes();
    void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client);
    void processJsonCommand(JsonDocument& doc, AsyncWebSocketClient *client);
    
    // Telemetry generation
    String getTelemetryJson();
    String getConfigJson();
};

#endif // WEB_SERVER_MANAGER_H
