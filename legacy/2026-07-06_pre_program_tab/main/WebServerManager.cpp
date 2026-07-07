#include "WebServerManager.h"

WebServerManager::WebServerManager(StorageManager& storage, MotorController& m1, MotorController& m2,
                                   RelayController& r1, RelayController& r2)
    : _storage(storage), _m1(m1), _m2(m2), _r1(r1), _r2(r2), _server(80), _ws("/ws"),
      _lastBroadcastTime(0), _hasPendingLog(false) {}

void WebServerManager::begin() {
    // 1. Wi-Fi Setup
    WiFi.mode(WIFI_AP_STA);
    
    // Set up Access Point
    WiFi.softAP(AP_SSID, AP_PASS);
    Serial.println("Access Point Started!");
    Serial.print("AP SSID: ");
    Serial.println(AP_SSID);
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    
    // Connect to configured STA Wi-Fi (non-blocking begin)
    _storage.loadWifiConfig(_wifiConfig);
    if (strlen(_wifiConfig.ssid) > 0) {
        WiFi.begin(_wifiConfig.ssid, _wifiConfig.password);
        Serial.print("Connecting to STA Wi-Fi: ");
        Serial.println(_wifiConfig.ssid);
    } else {
        Serial.println("No STA credentials configured. Operating in AP mode.");
    }
    
    // 2. WebSocket Setup
    _ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onWsEvent(server, client, type, arg, data, len);
    });
    _server.addHandler(&_ws);
    
    // 3. HTTP Routes Setup
    setupRoutes();
    
    // Start Server
    _server.begin();
    Serial.println("HTTP Web Server started on port 80");
}

void WebServerManager::update() {
    // Broadcast telemetry to WebSocket clients every 200 ms (5 Hz)
    if (millis() - _lastBroadcastTime > 200) {
        _lastBroadcastTime = millis();
        broadcastTelemetry();
    }
    
    // Clean up disconnected WS clients
    _ws.cleanupClients();
}

void WebServerManager::broadcastTelemetry() {
    if (_ws.count() > 0) {
        _ws.textAll(getTelemetryJson());
    }
}

void WebServerManager::queueLog(const String& message) {
    _pendingLog = message;
    _hasPendingLog = true;
    Serial.println("LOG: " + message);
}

void WebServerManager::setupRoutes() {
    // Dashboard Homepage
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", INDEX_HTML);
    });

    // REST: GET /status
    _server.on("/status", HTTP_GET, [this](AsyncWebServerRequest *request) {
        request->send(200, "application/json", getTelemetryJson());
    });

    // REST: POST /motor1/move
    _server.on("/motor1/move", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        #if ARDUINOJSON_VERSION_MAJOR >= 7
            JsonDocument doc;
        #else
            DynamicJsonDocument doc(1024);
        #endif
        DeserializationError error = deserializeJson(doc, data, len);
        if (!error) {
            float val = doc["val"] | 0.0f;
            if (_m1.moveTo(val)) {
                request->send(200, "application/json", "{\"status\":\"ok\"}");
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Soft limit violation\"}");
            }
        } else {
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"JSON parse error\"}");
        }
    });

    // REST: POST /motor2/move
    _server.on("/motor2/move", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        #if ARDUINOJSON_VERSION_MAJOR >= 7
            JsonDocument doc;
        #else
            DynamicJsonDocument doc(1024);
        #endif
        DeserializationError error = deserializeJson(doc, data, len);
        if (!error) {
            float val = doc["val"] | 0.0f;
            if (_m2.moveTo(val)) {
                request->send(200, "application/json", "{\"status\":\"ok\"}");
            } else {
                request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"Soft limit violation\"}");
            }
        } else {
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"JSON parse error\"}");
        }
    });

    // REST: POST /motor1/home
    _server.on("/motor1/home", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m1.startHoming();
        queueLog("Motor 1 homing started by REST.");
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor2/home
    _server.on("/motor2/home", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m2.startHoming();
        queueLog("Motor 2 homing started by REST.");
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor1/stop
    _server.on("/motor1/stop", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m1.stop();
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor2/stop
    _server.on("/motor2/stop", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m2.stop();
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor1/enable
    _server.on("/motor1/enable", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m1.enableDriver(true);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor2/enable
    _server.on("/motor2/enable", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m2.enableDriver(true);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor1/disable
    _server.on("/motor1/disable", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m1.enableDriver(false);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /motor2/disable
    _server.on("/motor2/disable", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m2.enableDriver(false);
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /allstop
    _server.on("/allstop", HTTP_POST, [this](AsyncWebServerRequest *request) {
        _m1.stop();
        _m2.stop();
        _r1.turnOff();
        _r2.turnOff();
        queueLog("Emergency stop received by REST.");
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    });

    // REST: POST /save
    _server.on("/save", HTTP_POST, [this](AsyncWebServerRequest *request) {
        bool ok = _storage.savePositions(_m1.getCurrentPosition(), _m2.getCurrentPosition());
        if (ok) {
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"NVS save error\"}");
        }
    });

    // REST: POST /reboot
    _server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{\"status\":\"rebooting\"}");
        delay(500);
        ESP.restart();
    });

    // REST: POST /wifi/save
    _server.on("/wifi/save", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        #if ARDUINOJSON_VERSION_MAJOR >= 7
            JsonDocument doc;
        #else
            DynamicJsonDocument doc(1024);
        #endif
        DeserializationError error = deserializeJson(doc, data, len);
        if (!error) {
            WifiConfig wifiCfg;
            String ssid = doc["ssid"] | "";
            String pass = doc["password"] | "";
            strncpy(wifiCfg.ssid, ssid.c_str(), sizeof(wifiCfg.ssid));
            strncpy(wifiCfg.password, pass.c_str(), sizeof(wifiCfg.password));
            if (_storage.saveWifiConfig(wifiCfg)) {
                request->send(200, "application/json", "{\"status\":\"ok\",\"message\":\"WiFi credentials saved. Reboot required.\"}");
            } else {
                request->send(500, "application/json", "{\"status\":\"error\",\"message\":\"NVS write failed\"}");
            }
        } else {
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"JSON parse error\"}");
        }
    });

    // REST: POST /config/save
    _server.on("/config/save", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        #if ARDUINOJSON_VERSION_MAJOR >= 7
            JsonDocument doc;
        #else
            DynamicJsonDocument doc(2048);
        #endif
        DeserializationError error = deserializeJson(doc, data, len);
        if (!error) {
            processJsonCommand(doc, nullptr);
            request->send(200, "application/json", "{\"status\":\"ok\"}");
        } else {
            request->send(400, "application/json", "{\"status\":\"error\",\"message\":\"JSON parse error\"}");
        }
    });
}

void WebServerManager::onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WS Client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        client->text(getConfigJson());
        client->text(getTelemetryJson());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WS Client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        handleWebSocketMessage(arg, data, len, client);
    }
}

void WebServerManager::handleWebSocketMessage(void *arg, uint8_t *data, size_t len, AsyncWebSocketClient *client) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        #if ARDUINOJSON_VERSION_MAJOR >= 7
            JsonDocument doc;
        #else
            DynamicJsonDocument doc(2048);
        #endif
        DeserializationError error = deserializeJson(doc, (const uint8_t*)data, len);
        if (!error) {
            processJsonCommand(doc, client);
        } else {
            Serial.print("WS JSON Deserialization failed: ");
            Serial.println(error.c_str());
        }
    }
}

void WebServerManager::processJsonCommand(JsonDocument& doc, AsyncWebSocketClient *client) {
    String cmd = doc["cmd"] | "";
    if (cmd == "move") {
        int motor = doc["motor"] | 1;
        float val = doc["val"] | 0.0f;
        if (motor == 1) {
            if (!_m1.moveTo(val)) {
                queueLog("Error: Motor 1 soft limit violation.");
            }
        } else {
            if (!_m2.moveTo(val)) {
                queueLog("Error: Motor 2 soft limit violation.");
            }
        }
    } 
    else if (cmd == "jog") {
        int motor = doc["motor"] | 1;
        float val = doc["val"] | 0.0f;
        if (motor == 1) {
            long steps = _m1.unitsToSteps(val);
            if (!_m1.moveRelative(steps)) {
                queueLog("Error: Motor 1 soft limit violation.");
            }
        } else {
            long steps = _m2.unitsToSteps(val);
            if (!_m2.moveRelative(steps)) {
                queueLog("Error: Motor 2 soft limit violation.");
            }
        }
    }
    else if (cmd == "stop") {
        int motor = doc["motor"] | 1;
        if (motor == 1) _m1.stop();
        else _m2.stop();
    }
    else if (cmd == "home") {
        int motor = doc["motor"] | 1;
        if (motor == 1) {
            queueLog("Motor 1 homing started.");
            _m1.startHoming();
        } else {
            queueLog("Motor 2 homing started.");
            _m2.startHoming();
        }
    }
    else if (cmd == "reset") {
        int motor = doc["motor"] | 1;
        if (motor == 1) {
            _m1.resetPosition();
            queueLog("Motor 1 position reset to zero.");
        } else {
            _m2.resetPosition();
            queueLog("Motor 2 position reset to zero.");
        }
    }
    else if (cmd == "enable") {
        int motor = doc["motor"] | 1;
        int val = doc["val"] | 1;
        if (motor == 1) _m1.enableDriver(val == 1);
        else _m2.enableDriver(val == 1);
    }
    else if (cmd == "allstop") {
        _m1.stop();
        _m2.stop();
        _r1.turnOff();
        _r2.turnOff();
        queueLog("Emergency stop received.");
    }
    else if (cmd == "relay_trigger") {
        int relay = doc["relay"] | 1;
        uint32_t duration = doc["duration"] | 1;
        if (duration > 86400) duration = 86400;

        if (relay == 1) {
            _r1.trigger(duration);
            queueLog("Relay 1 triggered for " + String(duration) + " seconds.");
        } else {
            _r2.trigger(duration);
            queueLog("Relay 2 triggered for " + String(duration) + " seconds.");
        }
    }
    else if (cmd == "relay_off") {
        int relay = doc["relay"] | 1;

        if (relay == 1) {
            _r1.turnOff();
            queueLog("Relay 1 turned off.");
        } else {
            _r2.turnOff();
            queueLog("Relay 2 turned off.");
        }
    }
    else if (cmd == "reboot") {
        delay(200);
        ESP.restart();
    }
    else if (cmd == "req_config") {
        if (client) {
            client->text(getConfigJson());
        }
    }
    else if (cmd == "save_config") {
        // Save Motor 1 Config
        if (doc.containsKey("m1")) {
            MotorConfig m1Cfg;
            _storage.loadMotorConfig(1, m1Cfg);
            m1Cfg.maxSpeed = doc["m1"]["maxSpeed"] | m1Cfg.maxSpeed;
            m1Cfg.acceleration = doc["m1"]["acceleration"] | m1Cfg.acceleration;
            m1Cfg.minPosition = doc["m1"]["minPosition"] | m1Cfg.minPosition;
            m1Cfg.maxPosition = doc["m1"]["maxPosition"] | m1Cfg.maxPosition;
            m1Cfg.stepAngle = doc["m1"]["stepAngle"] | m1Cfg.stepAngle;
            m1Cfg.microsteps = doc["m1"]["microsteps"] | m1Cfg.microsteps;
            m1Cfg.driveType = doc["m1"]["driveType"] | m1Cfg.driveType;
            m1Cfg.leadScrewPitch = doc["m1"]["leadScrewPitch"] | m1Cfg.leadScrewPitch;
            m1Cfg.beltPitch = doc["m1"]["beltPitch"] | m1Cfg.beltPitch;
            m1Cfg.pulleyTeeth = doc["m1"]["pulleyTeeth"] | m1Cfg.pulleyTeeth;
            m1Cfg.unitType = doc["m1"]["unitType"] | m1Cfg.unitType;
            _storage.saveMotorConfig(1, m1Cfg);
            _m1.updateConfig(m1Cfg);
        }
        // Save Motor 2 Config
        if (doc.containsKey("m2")) {
            MotorConfig m2Cfg;
            _storage.loadMotorConfig(2, m2Cfg);
            m2Cfg.maxSpeed = doc["m2"]["maxSpeed"] | m2Cfg.maxSpeed;
            m2Cfg.acceleration = doc["m2"]["acceleration"] | m2Cfg.acceleration;
            m2Cfg.minPosition = doc["m2"]["minPosition"] | m2Cfg.minPosition;
            m2Cfg.maxPosition = doc["m2"]["maxPosition"] | m2Cfg.maxPosition;
            m2Cfg.stepAngle = doc["m2"]["stepAngle"] | m2Cfg.stepAngle;
            m2Cfg.microsteps = doc["m2"]["microsteps"] | m2Cfg.microsteps;
            m2Cfg.driveType = doc["m2"]["driveType"] | m2Cfg.driveType;
            m2Cfg.leadScrewPitch = doc["m2"]["leadScrewPitch"] | m2Cfg.leadScrewPitch;
            m2Cfg.beltPitch = doc["m2"]["beltPitch"] | m2Cfg.beltPitch;
            m2Cfg.pulleyTeeth = doc["m2"]["pulleyTeeth"] | m2Cfg.pulleyTeeth;
            m2Cfg.unitType = doc["m2"]["unitType"] | m2Cfg.unitType;
            _storage.saveMotorConfig(2, m2Cfg);
            _m2.updateConfig(m2Cfg);
        }
        // Save System settings
        if (doc.containsKey("sys")) {
            SystemConfig sysCfg;
            _storage.loadSystemConfig(sysCfg);
            sysCfg.enableTimeoutActive = doc["sys"]["timeoutActive"] | sysCfg.enableTimeoutActive;
            sysCfg.enableTimeoutDuration = doc["sys"]["timeoutDuration"] | sysCfg.enableTimeoutDuration;
            _storage.saveSystemConfig(sysCfg);
            _m1.setEnableTimeout(sysCfg.enableTimeoutActive, sysCfg.enableTimeoutDuration);
            _m2.setEnableTimeout(sysCfg.enableTimeoutActive, sysCfg.enableTimeoutDuration);
        }
        queueLog("System settings saved.");
        _ws.textAll(getConfigJson()); // broadcast updated config to all
    }
    else if (cmd == "save_wifi") {
        WifiConfig wifiCfg;
        String ssid = doc["ssid"] | "";
        String pass = doc["pass"] | "";
        strncpy(wifiCfg.ssid, ssid.c_str(), sizeof(wifiCfg.ssid));
        strncpy(wifiCfg.password, pass.c_str(), sizeof(wifiCfg.password));
        _storage.saveWifiConfig(wifiCfg);
        queueLog("WiFi settings saved. Reboot required.");
    }
}

String WebServerManager::getTelemetryJson() {
    #if ARDUINOJSON_VERSION_MAJOR >= 7
        JsonDocument doc;
    #else
        DynamicJsonDocument doc(2048);
    #endif
    
    // Motor 1 status
    doc["m1Pos"] = _m1.getCurrentPosition();
    doc["m1PosU"] = _m1.getCurrentPositionUnit();
    doc["m1Target"] = _m1.getTargetPosition();
    doc["m1TargetU"] = _m1.getTargetPositionUnit();
    doc["m1Dist"] = _m1.getRemainingDistance();
    doc["m1DistU"] = _m1.getRemainingDistanceUnit();
    doc["m1Speed"] = _m1.getSpeed();
    doc["m1Accel"] = _m1.getAcceleration();
    
    int m1Status = 0;
    if (!_m1.isDriverEnabled()) m1Status = 0;
    else if (_m1.isHoming()) m1Status = 3;
    else if (_m1.isRunning()) m1Status = 2;
    else m1Status = 1;
    doc["m1Status"] = m1Status;
    
    doc["m1Sw"] = _m1.isLimitSwitchActive();
    doc["m1En"] = _m1.isDriverEnabled();
    doc["m1HomeOk"] = _m1.isHomeCompleted();
    
    long m1Remain = _m1.getRemainingDistance();
    if (m1Remain == 0) {
        doc["m1Percent"] = 100;
    } else {
        long m1Target = _m1.getTargetPosition();
        if (m1Target == 0) {
            doc["m1Percent"] = (_m1.getCurrentPosition() == 0) ? 100 : 0;
        } else {
            double pct = (1.0 - (double)m1Remain / abs(m1Target)) * 100.0;
            if (pct < 0) pct = 0;
            if (pct > 100) pct = 100;
            doc["m1Percent"] = (int)pct;
        }
    }
    
    // Motor 2 status
    doc["m2Pos"] = _m2.getCurrentPosition();
    doc["m2PosU"] = _m2.getCurrentPositionUnit();
    doc["m2Target"] = _m2.getTargetPosition();
    doc["m2TargetU"] = _m2.getTargetPositionUnit();
    doc["m2Dist"] = _m2.getRemainingDistance();
    doc["m2DistU"] = _m2.getRemainingDistanceUnit();
    doc["m2Speed"] = _m2.getSpeed();
    doc["m2Accel"] = _m2.getAcceleration();
    
    int m2Status = 0;
    if (!_m2.isDriverEnabled()) m2Status = 0;
    else if (_m2.isHoming()) m2Status = 3;
    else if (_m2.isRunning()) m2Status = 2;
    else m2Status = 1;
    doc["m2Status"] = m2Status;
    
    doc["m2Sw"] = _m2.isLimitSwitchActive();
    doc["m2En"] = _m2.isDriverEnabled();
    doc["m2HomeOk"] = _m2.isHomeCompleted();
    
    long m2Remain = _m2.getRemainingDistance();
    if (m2Remain == 0) {
        doc["m2Percent"] = 100;
    } else {
        long m2Target = _m2.getTargetPosition();
        if (m2Target == 0) {
            doc["m2Percent"] = (_m2.getCurrentPosition() == 0) ? 100 : 0;
        } else {
            double pct = (1.0 - (double)m2Remain / abs(m2Target)) * 100.0;
            if (pct < 0) pct = 0;
            if (pct > 100) pct = 100;
            doc["m2Percent"] = (int)pct;
        }
    }
    
    // System fields
    MotorConfig m1c, m2c;
    _storage.loadMotorConfig(1, m1c);
    _storage.loadMotorConfig(2, m2c);
    doc["m1Unit"] = m1c.unitType;
    doc["m2Unit"] = m2c.unitType;
    
    doc["wifiMode"] = (WiFi.getMode() == WIFI_AP_STA) ? "AP+STA" : "AP";
    doc["wifiIP"] = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
    doc["uptime"] = millis() / 1000;
    doc["heap"] = ESP.getFreeHeap();
    doc["wdt"] = true; // Task watchdog is running and checked in main.ino
    doc["r1Active"] = _r1.isActive();
    doc["r1Remaining"] = _r1.getRemainingSeconds();
    doc["r1Pin"] = _r1.getPin();
    doc["r2Active"] = _r2.isActive();
    doc["r2Remaining"] = _r2.getRemainingSeconds();
    doc["r2Pin"] = _r2.getPin();
    
    if (_hasPendingLog) {
        doc["log"] = _pendingLog;
        _hasPendingLog = false;
    }
    
    String output;
    serializeJson(doc, output);
    return output;
}

String WebServerManager::getConfigJson() {
    #if ARDUINOJSON_VERSION_MAJOR >= 7
        JsonDocument doc;
    #else
        DynamicJsonDocument doc(2048);
    #endif
    
    doc["configUpdate"] = true;
    
    MotorConfig m1Cfg, m2Cfg;
    _storage.loadMotorConfig(1, m1Cfg);
    _storage.loadMotorConfig(2, m2Cfg);
    
    SystemConfig sysCfg;
    _storage.loadSystemConfig(sysCfg);
    
    doc["m1Cfg"]["maxSpeed"] = m1Cfg.maxSpeed;
    doc["m1Cfg"]["acceleration"] = m1Cfg.acceleration;
    doc["m1Cfg"]["minPosition"] = m1Cfg.minPosition;
    doc["m1Cfg"]["maxPosition"] = m1Cfg.maxPosition;
    doc["m1Cfg"]["stepAngle"] = m1Cfg.stepAngle;
    doc["m1Cfg"]["microsteps"] = m1Cfg.microsteps;
    doc["m1Cfg"]["driveType"] = m1Cfg.driveType;
    doc["m1Cfg"]["leadScrewPitch"] = m1Cfg.leadScrewPitch;
    doc["m1Cfg"]["beltPitch"] = m1Cfg.beltPitch;
    doc["m1Cfg"]["pulleyTeeth"] = m1Cfg.pulleyTeeth;
    doc["m1Cfg"]["unitType"] = m1Cfg.unitType;
    
    doc["m2Cfg"]["maxSpeed"] = m2Cfg.maxSpeed;
    doc["m2Cfg"]["acceleration"] = m2Cfg.acceleration;
    doc["m2Cfg"]["minPosition"] = m2Cfg.minPosition;
    doc["m2Cfg"]["maxPosition"] = m2Cfg.maxPosition;
    doc["m2Cfg"]["stepAngle"] = m2Cfg.stepAngle;
    doc["m2Cfg"]["microsteps"] = m2Cfg.microsteps;
    doc["m2Cfg"]["driveType"] = m2Cfg.driveType;
    doc["m2Cfg"]["leadScrewPitch"] = m2Cfg.leadScrewPitch;
    doc["m2Cfg"]["beltPitch"] = m2Cfg.beltPitch;
    doc["m2Cfg"]["pulleyTeeth"] = m2Cfg.pulleyTeeth;
    doc["m2Cfg"]["unitType"] = m2Cfg.unitType;
    
    doc["sysCfg"]["timeoutActive"] = sysCfg.enableTimeoutActive;
    doc["sysCfg"]["timeoutDuration"] = sysCfg.enableTimeoutDuration;
    
    String output;
    serializeJson(doc, output);
    return output;
}
