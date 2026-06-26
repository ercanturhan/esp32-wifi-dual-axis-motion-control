#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Dual Axis Motion Control Dashboard</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-color: #0b0f19;
            --card-bg: rgba(20, 30, 55, 0.65);
            --card-border: rgba(255, 255, 255, 0.08);
            --primary: #3b82f6;
            --primary-glow: rgba(59, 130, 246, 0.4);
            --success: #10b981;
            --success-glow: rgba(16, 185, 129, 0.4);
            --danger: #ef4444;
            --danger-glow: rgba(239, 68, 68, 0.5);
            --warning: #f59e0b;
            --text-main: #f3f4f6;
            --text-muted: #9ca3af;
            --accent: #8b5cf6;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            font-family: 'Outfit', sans-serif;
            -webkit-tap-highlight-color: transparent;
        }

        body {
            background-color: var(--bg-color);
            background-image: 
                radial-gradient(at 0% 0%, rgba(59, 130, 246, 0.15) 0px, transparent 50%),
                radial-gradient(at 100% 100%, rgba(139, 92, 246, 0.15) 0px, transparent 50%);
            background-attachment: fixed;
            color: var(--text-main);
            min-height: 100vh;
            display: flex;
            flex-direction: column;
            padding: 20px;
        }

        header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding-bottom: 20px;
            border-bottom: 1px solid var(--card-border);
            margin-bottom: 20px;
            flex-wrap: wrap;
            gap: 15px;
        }

        .logo-area h1 {
            font-size: 1.5rem;
            font-weight: 700;
            background: linear-gradient(135deg, #60a5fa, #c084fc);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            display: flex;
            align-items: center;
            gap: 10px;
        }

        .status-badge {
            display: inline-flex;
            align-items: center;
            gap: 6px;
            padding: 6px 12px;
            border-radius: 9999px;
            font-size: 0.85rem;
            font-weight: 500;
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid var(--card-border);
        }

        .status-dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background-color: var(--danger);
        }

        .status-dot.connected {
            background-color: var(--success);
            box-shadow: 0 0 8px var(--success-glow);
        }

        .emergency-btn {
            background: linear-gradient(135deg, #ef4444, #b91c1c);
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 12px;
            font-weight: 700;
            font-size: 1rem;
            cursor: pointer;
            box-shadow: 0 4px 15px var(--danger-glow);
            transition: all 0.2s ease;
            text-transform: uppercase;
            letter-spacing: 0.05em;
        }

        .emergency-btn:hover {
            transform: scale(1.03);
            box-shadow: 0 6px 20px var(--danger-glow);
        }

        .emergency-btn:active {
            transform: scale(0.97);
        }

        .dashboard-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
            gap: 20px;
            flex-grow: 1;
        }

        .card {
            background: var(--card-bg);
            border: 1px solid var(--card-border);
            border-radius: 20px;
            padding: 20px;
            backdrop-filter: blur(12px);
            -webkit-backdrop-filter: blur(12px);
            display: flex;
            flex-direction: column;
            gap: 15px;
            transition: border-color 0.3s ease;
        }

        .card:hover {
            border-color: rgba(255, 255, 255, 0.15);
        }

        .card-title {
            font-size: 1.15rem;
            font-weight: 600;
            color: var(--text-main);
            display: flex;
            justify-content: space-between;
            align-items: center;
            border-bottom: 1px solid rgba(255, 255, 255, 0.05);
            padding-bottom: 10px;
        }

        .card-subtitle {
            font-size: 0.85rem;
            color: var(--text-muted);
        }

        .stat-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 12px;
        }

        .stat-box {
            background: rgba(255, 255, 255, 0.03);
            border: 1px solid rgba(255, 255, 255, 0.05);
            padding: 10px;
            border-radius: 12px;
            display: flex;
            flex-direction: column;
            gap: 4px;
        }

        .stat-label {
            font-size: 0.75rem;
            color: var(--text-muted);
            text-transform: uppercase;
        }

        .stat-value {
            font-size: 1.2rem;
            font-weight: 700;
            color: var(--text-main);
        }

        .stat-unit {
            font-size: 0.8rem;
            font-weight: 400;
            color: var(--text-muted);
            margin-left: 2px;
        }

        .motor-controls {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 10px;
        }

        .btn {
            background: rgba(255, 255, 255, 0.06);
            border: 1px solid rgba(255, 255, 255, 0.1);
            color: var(--text-main);
            padding: 10px;
            border-radius: 10px;
            font-size: 0.9rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s ease;
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
        }

        .btn:hover {
            background: rgba(255, 255, 255, 0.15);
            border-color: rgba(255, 255, 255, 0.2);
        }

        .btn:active {
            transform: scale(0.97);
        }

        .btn-primary {
            background: linear-gradient(135deg, #2563eb, #1d4ed8);
            border: none;
            box-shadow: 0 4px 10px rgba(37, 99, 235, 0.3);
        }

        .btn-primary:hover {
            background: linear-gradient(135deg, #3b82f6, #2563eb);
            box-shadow: 0 6px 14px rgba(37, 99, 235, 0.4);
        }

        .btn-success {
            background: linear-gradient(135deg, #059669, #047857);
            border: none;
            box-shadow: 0 4px 10px rgba(5, 150, 105, 0.3);
        }

        .btn-success:hover {
            background: linear-gradient(135deg, #10b981, #059669);
        }

        .btn-warning {
            background: linear-gradient(135deg, #d97706, #b45309);
            border: none;
        }

        .btn-warning:hover {
            background: linear-gradient(135deg, #f59e0b, #d97706);
        }

        .btn-danger {
            background: linear-gradient(135deg, #dc2626, #b91c1c);
            border: none;
        }

        .btn-danger:hover {
            background: linear-gradient(135deg, #ef4444, #dc2626);
        }

        .input-group {
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .input-group label {
            font-size: 0.8rem;
            color: var(--text-muted);
            font-weight: 500;
        }

        .input-with-btn {
            display: flex;
            gap: 8px;
        }

        .input-with-btn input {
            flex-grow: 1;
        }

        input, select {
            background: rgba(255, 255, 255, 0.04);
            border: 1px solid rgba(255, 255, 255, 0.08);
            border-radius: 10px;
            padding: 10px 12px;
            color: var(--text-main);
            font-size: 0.95rem;
            transition: all 0.2s ease;
            outline: none;
        }

        input:focus, select:focus {
            border-color: var(--primary);
            background: rgba(255, 255, 255, 0.07);
            box-shadow: 0 0 0 3px var(--primary-glow);
        }

        .progress-container {
            width: 100%;
            background: rgba(255, 255, 255, 0.05);
            border-radius: 9999px;
            height: 10px;
            overflow: hidden;
            position: relative;
        }

        .progress-bar {
            height: 100%;
            width: 0%;
            background: linear-gradient(90deg, var(--primary), var(--accent));
            border-radius: 9999px;
            transition: width 0.3s ease;
        }

        .badge {
            padding: 3px 8px;
            border-radius: 6px;
            font-size: 0.75rem;
            font-weight: 600;
            text-transform: uppercase;
        }

        .badge-idle { background: rgba(59, 130, 246, 0.15); color: #60a5fa; }
        .badge-moving { background: rgba(16, 185, 129, 0.15); color: #34d399; }
        .badge-homing { background: rgba(245, 158, 11, 0.15); color: #fbbf24; }
        .badge-disabled { background: rgba(156, 163, 175, 0.15); color: #d1d5db; }
        .badge-error { background: rgba(239, 68, 68, 0.15); color: #fca5a5; }

        .indicator {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            display: inline-block;
            background: rgba(255, 255, 255, 0.1);
        }

        .indicator.active {
            background: var(--success);
            box-shadow: 0 0 8px var(--success-glow);
        }

        .tabs {
            display: flex;
            gap: 10px;
            margin-bottom: 20px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.05);
            padding-bottom: 10px;
        }

        .tab {
            padding: 10px 20px;
            border-radius: 10px;
            cursor: pointer;
            font-weight: 500;
            background: rgba(255, 255, 255, 0.02);
            border: 1px solid rgba(255, 255, 255, 0.05);
            color: var(--text-muted);
            transition: all 0.2s ease;
        }

        .tab.active {
            background: var(--primary);
            color: white;
            border-color: transparent;
            box-shadow: 0 4px 12px var(--primary-glow);
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
        }

        /* Log console */
        .console {
            background: #040810;
            border: 1px solid var(--card-border);
            border-radius: 12px;
            font-family: 'Courier New', Courier, monospace;
            padding: 15px;
            height: 150px;
            overflow-y: auto;
            color: #34d399;
            font-size: 0.85rem;
            line-height: 1.4;
            display: flex;
            flex-direction: column;
            gap: 4px;
        }

        .console-entry {
            display: flex;
            gap: 8px;
        }

        .console-time {
            color: #60a5fa;
            flex-shrink: 0;
        }

        .console-text {
            word-break: break-all;
        }

        .console-warn { color: var(--warning); }
        .console-err { color: var(--danger); }

        .settings-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(280px, 1fr));
            gap: 20px;
        }

        footer {
            text-align: center;
            padding: 30px 0 10px 0;
            font-size: 0.8rem;
            color: var(--text-muted);
        }

        .unit-selector {
            width: 100%;
        }

        .jog-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 8px;
        }

        .wdt-status {
            display: flex;
            align-items: center;
            gap: 6px;
            font-size: 0.85rem;
        }

        @media (max-width: 768px) {
            body {
                padding: 10px;
            }
            .dashboard-grid {
                grid-template-columns: 1fr;
            }
            .stat-grid {
                grid-template-columns: 1fr 1fr;
            }
            header {
                flex-direction: column;
                align-items: stretch;
            }
            .logo-area {
                text-align: center;
                display: flex;
                justify-content: center;
            }
            .header-actions {
                display: flex;
                flex-direction: column;
                gap: 10px;
            }
        }
    </style>
</head>
<body>

    <header>
        <div class="logo-area">
            <h1>🌀 Dual Axis Motion Control</h1>
        </div>
        <div class="header-actions">
            <span class="status-badge">
                <span id="ws-status-dot" class="status-dot"></span>
                <span id="ws-status-text">WebSocket: Bağlantı Yok</span>
            </span>
            <button class="emergency-btn" onclick="sendAllStop()">ALL STOP (ACİL DUR)</button>
        </div>
    </header>

    <div class="tabs">
        <div class="tab active" onclick="switchTab('dashboard')">Gösterge Paneli</div>
        <div class="tab" onclick="switchTab('settings')">Motor Ayarları</div>
        <div class="tab" onclick="switchTab('wifi-tab')">WiFi Ayarları</div>
    </div>

    <!-- TAB: DASHBOARD -->
    <div id="tab-dashboard" class="tab-content active">
        <div class="dashboard-grid">
            
            <!-- MOTOR 1 -->
            <div class="card">
                <div class="card-title">
                    <div>
                        <h2>X Ekseni (Motor 1)</h2>
                        <span class="card-subtitle">GPIO25/26 - Lead Screw / Belt</span>
                    </div>
                    <span id="m1-status-badge" class="badge badge-disabled">Pasif</span>
                </div>

                <div class="stat-grid">
                    <div class="stat-box">
                        <span class="stat-label">Anlık Pozisyon</span>
                        <span class="stat-value"><span id="m1-pos-val">0.00</span><span id="m1-pos-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label" style="font-size: 0.65rem;">Pulse: <span id="m1-pos-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Hedef Pozisyon</span>
                        <span class="stat-value"><span id="m1-target-val">0.00</span><span id="m1-target-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label" style="font-size: 0.65rem;">Pulse: <span id="m1-target-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Kalan Mesafe</span>
                        <span class="stat-value"><span id="m1-dist-val">0.00</span><span id="m1-dist-unit" class="stat-unit">mm</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Hız / İvme</span>
                        <span class="stat-value" style="font-size: 0.95rem; line-height: 1.2;">
                            Hız: <span id="m1-speed">0</span> Hz<br>
                            İvme: <span id="m1-accel">0</span> Hz/s²
                        </span>
                    </div>
                </div>

                <!-- Progress percentage -->
                <div style="display:flex; justify-content:space-between; font-size:0.8rem; color:var(--text-muted);">
                    <span>İlerleme</span>
                    <span id="m1-progress-pct">100%</span>
                </div>
                <div class="progress-container">
                    <div id="m1-progress-bar" class="progress-bar"></div>
                </div>

                <div style="display: flex; justify-content: space-between; font-size: 0.85rem; background: rgba(255,255,255,0.02); padding: 8px; border-radius: 8px;">
                    <div>Home Switch: <span id="m1-sw" class="indicator"></span></div>
                    <div>Sürücü Yetkisi: <span id="m1-en" class="indicator"></span></div>
                    <div>Home Durumu: <span id="m1-home-ok" style="font-weight:600; color:var(--danger)">Bulunmadı</span></div>
                </div>

                <div class="input-group">
                    <label>Hedef Pozisyon Git</label>
                    <div class="input-with-btn">
                        <input type="number" id="m1-goto-input" step="0.01" placeholder="Miktar...">
                        <button class="btn btn-primary" onclick="moveMotor(1)">GİT</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button class="btn btn-warning" onclick="jogMotor(1, -10)">-10</button>
                    <button class="btn" style="color:var(--danger); border-color:var(--danger-glow)" onclick="stopMotor(1)">DUR</button>
                    <button class="btn btn-success" onclick="jogMotor(1, 10)">+10</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-primary" onclick="homeMotor(1)">🚀 HOME YAP</button>
                    <button class="btn" onclick="resetMotorPos(1)">📍 0 NOKTASI YAP</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-success" onclick="enableMotor(1, true)">Güç Ver (EN)</button>
                    <button class="btn btn-danger" onclick="enableMotor(1, false)">Gücü Kes (DIS)</button>
                </div>
            </div>

            <!-- MOTOR 2 -->
            <div class="card">
                <div class="card-title">
                    <div>
                        <h2>Y Ekseni (Motor 2)</h2>
                        <span class="card-subtitle">GPIO18/19 - Lead Screw / Belt</span>
                    </div>
                    <span id="m2-status-badge" class="badge badge-disabled">Pasif</span>
                </div>

                <div class="stat-grid">
                    <div class="stat-box">
                        <span class="stat-label">Anlık Pozisyon</span>
                        <span class="stat-value"><span id="m2-pos-val">0.00</span><span id="m2-pos-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label" style="font-size: 0.65rem;">Pulse: <span id="m2-pos-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Hedef Pozisyon</span>
                        <span class="stat-value"><span id="m2-target-val">0.00</span><span id="m2-target-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label" style="font-size: 0.65rem;">Pulse: <span id="m2-target-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Kalan Mesafe</span>
                        <span class="stat-value"><span id="m2-dist-val">0.00</span><span id="m2-dist-unit" class="stat-unit">mm</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Hız / İvme</span>
                        <span class="stat-value" style="font-size: 0.95rem; line-height: 1.2;">
                            Hız: <span id="m2-speed">0</span> Hz<br>
                            İvme: <span id="m2-accel">0</span> Hz/s²
                        </span>
                    </div>
                </div>

                <!-- Progress percentage -->
                <div style="display:flex; justify-content:space-between; font-size:0.8rem; color:var(--text-muted);">
                    <span>İlerleme</span>
                    <span id="m2-progress-pct">100%</span>
                </div>
                <div class="progress-container">
                    <div id="m2-progress-bar" class="progress-bar"></div>
                </div>

                <div style="display: flex; justify-content: space-between; font-size: 0.85rem; background: rgba(255,255,255,0.02); padding: 8px; border-radius: 8px;">
                    <div>Home Switch: <span id="m2-sw" class="indicator"></span></div>
                    <div>Sürücü Yetkisi: <span id="m2-en" class="indicator"></span></div>
                    <div>Home Durumu: <span id="m2-home-ok" style="font-weight:600; color:var(--danger)">Bulunmadı</span></div>
                </div>

                <div class="input-group">
                    <label>Hedef Pozisyon Git</label>
                    <div class="input-with-btn">
                        <input type="number" id="m2-goto-input" step="0.01" placeholder="Miktar...">
                        <button class="btn btn-primary" onclick="moveMotor(2)">GİT</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button class="btn btn-warning" onclick="jogMotor(2, -10)">-10</button>
                    <button class="btn" style="color:var(--danger); border-color:var(--danger-glow)" onclick="stopMotor(2)">DUR</button>
                    <button class="btn btn-success" onclick="jogMotor(2, 10)">+10</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-primary" onclick="homeMotor(2)">🚀 HOME YAP</button>
                    <button class="btn" onclick="resetMotorPos(2)">📍 0 NOKTASI YAP</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-success" onclick="enableMotor(2, true)">Güç Ver (EN)</button>
                    <button class="btn btn-danger" onclick="enableMotor(2, false)">Gücü Kes (DIS)</button>
                </div>
            </div>

            <!-- TELEMETRY & CONSOLE -->
            <div class="card">
                <div class="card-title">
                    <h2>Sistem Bilgileri</h2>
                    <span class="wdt-status">WDT: <b id="sys-wdt" style="color:var(--success)">AKTİF</b></span>
                </div>
                <div class="stat-grid">
                    <div class="stat-box">
                        <span class="stat-label">Bağlantı Türü</span>
                        <span class="stat-value" id="sys-wifi-mode" style="font-size:1.1rem">-</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Sistem IP Adresi</span>
                        <span class="stat-value" id="sys-wifi-ip" style="font-size:1.1rem">-</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Uptime (Çalışma)</span>
                        <span class="stat-value" id="sys-uptime" style="font-size:1.1rem">0 s</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Boş Heap (RAM)</span>
                        <span class="stat-value" id="sys-heap" style="font-size:1.1rem">0 KB</span>
                    </div>
                </div>

                <div class="card-title" style="border:none; padding: 0; margin-top:10px;">
                    <h2>Sistem Günlüğü (Log)</h2>
                </div>
                <div class="console" id="sys-console">
                    <div class="console-entry">
                        <span class="console-time">[00:00:00]</span>
                        <span class="console-text">Arayüz başlatıldı. WebSocket bağlantısı bekleniyor...</span>
                    </div>
                </div>
                <button class="btn" onclick="document.getElementById('sys-console').innerHTML=''">Günlüğü Temizle</button>
            </div>

        </div>
    </div>

    <!-- TAB: SETTINGS -->
    <div id="tab-settings" class="tab-content">
        <div class="card">
            <div class="card-title">
                <h2>Motor Mekanik ve Limit Konfigürasyonları</h2>
            </div>
            
            <div class="settings-grid">
                <!-- MOTOR 1 SETTINGS -->
                <div style="display:flex; flex-direction:column; gap:12px; border-right: 1px solid rgba(255,255,255,0.05); padding-right:20px;">
                    <h3>Motor 1 (X Ekseni)</h3>
                    
                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Hız Limiti (Step/s)</label>
                            <input type="number" id="m1-cfg-speed" value="2000">
                        </div>
                        <div class="input-group">
                            <label>İvme (Step/s²)</label>
                            <input type="number" id="m1-cfg-accel" value="1000">
                        </div>
                    </div>

                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Yazılımsal Min Sınır (Step)</label>
                            <input type="number" id="m1-cfg-min" value="0">
                        </div>
                        <div class="input-group">
                            <label>Yazılımsal Max Sınır (Step)</label>
                            <input type="number" id="m1-cfg-max" value="50000">
                        </div>
                    </div>

                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Adım Açısı (Derece)</label>
                            <input type="number" id="m1-cfg-angle" step="0.1" value="1.8">
                        </div>
                        <div class="input-group">
                            <label>Microstep (1/x)</label>
                            <select id="m1-cfg-usteps">
                                <option value="1">1 (Full)</option>
                                <option value="2">2 (1/2)</option>
                                <option value="4">4 (1/4)</option>
                                <option value="8">8 (1/8)</option>
                                <option value="16" selected>16 (1/16)</option>
                                <option value="32">32 (1/32)</option>
                            </select>
                        </div>
                    </div>

                    <div class="input-group">
                        <label>Tahrik Sistemi</label>
                        <select id="m1-cfg-drive" onchange="toggleDriveInputs(1)">
                            <option value="0" selected>Vidalı Mil (Lead Screw)</option>
                            <option value="1">Kayış & Kasnak (Belt & Pulley)</option>
                        </select>
                    </div>

                    <div id="m1-drive-screw" class="input-group">
                        <label>Vida Hatvesi (mm/Tur)</label>
                        <input type="number" id="m1-cfg-pitch" step="0.01" value="8.00">
                    </div>

                    <div id="m1-drive-belt" class="stat-grid" style="display:none;">
                        <div class="input-group">
                            <label>Diş Hatvesi (mm)</label>
                            <input type="number" id="m1-cfg-beltpitch" step="0.01" value="2.00">
                        </div>
                        <div class="input-group">
                            <label>Diş Sayısı (T)</label>
                            <input type="number" id="m1-cfg-teeth" value="20">
                        </div>
                    </div>

                    <div class="input-group">
                        <label>Arayüz Birimi</label>
                        <select id="m1-cfg-unit" class="unit-selector">
                            <option value="0">Pulse (Step Adedi)</option>
                            <option value="1" selected>Milimetre (mm)</option>
                            <option value="2">Santimetre (cm)</option>
                            <option value="3">Derece (Degree)</option>
                        </select>
                    </div>
                </div>

                <!-- MOTOR 2 SETTINGS -->
                <div style="display:flex; flex-direction:column; gap:12px;">
                    <h3>Motor 2 (Y Ekseni)</h3>
                    
                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Hız Limiti (Step/s)</label>
                            <input type="number" id="m2-cfg-speed" value="2000">
                        </div>
                        <div class="input-group">
                            <label>İvme (Step/s²)</label>
                            <input type="number" id="m2-cfg-accel" value="1000">
                        </div>
                    </div>

                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Yazılımsal Min Sınır (Step)</label>
                            <input type="number" id="m2-cfg-min" value="0">
                        </div>
                        <div class="input-group">
                            <label>Yazılımsal Max Sınır (Step)</label>
                            <input type="number" id="m2-cfg-max" value="100000">
                        </div>
                    </div>

                    <div class="stat-grid">
                        <div class="input-group">
                            <label>Adım Açısı (Derece)</label>
                            <input type="number" id="m2-cfg-angle" step="0.1" value="1.8">
                        </div>
                        <div class="input-group">
                            <label>Microstep (1/x)</label>
                            <select id="m2-cfg-usteps">
                                <option value="1">1 (Full)</option>
                                <option value="2">2 (1/2)</option>
                                <option value="4">4 (1/4)</option>
                                <option value="8">8 (1/8)</option>
                                <option value="16" selected>16 (1/16)</option>
                                <option value="32">32 (1/32)</option>
                            </select>
                        </div>
                    </div>

                    <div class="input-group">
                        <label>Tahrik Sistemi</label>
                        <select id="m2-cfg-drive" onchange="toggleDriveInputs(2)">
                            <option value="0" selected>Vidalı Mil (Lead Screw)</option>
                            <option value="1">Kayış & Kasnak (Belt & Pulley)</option>
                        </select>
                    </div>

                    <div id="m2-drive-screw" class="input-group">
                        <label>Vida Hatvesi (mm/Tur)</label>
                        <input type="number" id="m2-cfg-pitch" step="0.01" value="8.00">
                    </div>

                    <div id="m2-drive-belt" class="stat-grid" style="display:none;">
                        <div class="input-group">
                            <label>Diş Hatvesi (mm)</label>
                            <input type="number" id="m2-cfg-beltpitch" step="0.01" value="2.00">
                        </div>
                        <div class="input-group">
                            <label>Diş Sayısı (T)</label>
                            <input type="number" id="m2-cfg-teeth" value="20">
                        </div>
                    </div>

                    <div class="input-group">
                        <label>Arayüz Birimi</label>
                        <select id="m2-cfg-unit" class="unit-selector">
                            <option value="0">Pulse (Step Adedi)</option>
                            <option value="1" selected>Milimetre (mm)</option>
                            <option value="2">Santimetre (cm)</option>
                            <option value="3">Derece (Degree)</option>
                        </select>
                    </div>
                </div>
            </div>

            <div style="border-top: 1px solid rgba(255,255,255,0.05); padding-top:15px; display:flex; flex-direction:column; gap:12px;">
                <h3>Sürücü Güç Yönetimi (Auto-Enable Timeout)</h3>
                <div class="stat-grid">
                    <div class="input-group">
                        <label>Timeout Özelliği</label>
                        <select id="sys-cfg-timeout-active">
                            <option value="1" selected>Aktif (10sn sonra enerjiyi kes)</option>
                            <option value="0">Pasif (Motor sürekli enerjili kalsın)</option>
                        </select>
                    </div>
                    <div class="input-group">
                        <label>Timeout Süresi (Saniye)</label>
                        <input type="number" id="sys-cfg-timeout-duration" value="10" min="1">
                    </div>
                </div>
            </div>

            <div style="display:flex; justify-content:flex-end; gap:10px; margin-top:20px;">
                <button class="btn btn-primary" style="padding:12px 24px;" onclick="saveMotorSettings()">AYARLARI KAYDET VE UYGULA</button>
            </div>
        </div>
    </div>

    <!-- TAB: WIFI -->
    <div id="tab-wifi-tab" class="tab-content">
        <div class="card">
            <div class="card-title">
                <h2>Wi-Fi Bağlantı Ayarları</h2>
            </div>
            
            <div style="display:flex; flex-direction:column; gap:15px; max-width:500px; margin:0 auto; width:100%;">
                <div class="stat-box" style="background:rgba(59,130,246,0.05); border-color:var(--primary-glow); padding:15px; margin-bottom:10px;">
                    <h3 style="color:#60a5fa; margin-bottom:5px;">Bilgilendirme</h3>
                    <p style="font-size:0.85rem; line-height:1.4; color:var(--text-muted)">
                        ESP32 varsayılan olarak hem kendi yayını yapar (<b>SSID: ESP32-MOTOR</b>, Şifre: <b>12345678</b>) hem de buraya girdiğiniz Wi-Fi ağına bağlanmaya çalışır. 
                        Bağlantı başarılı olduğunda, yerel ağınızdaki IP adresi ile sisteme ulaşabilirsiniz.
                    </p>
                </div>

                <div class="input-group">
                    <label>Kablosuz Ağ Adı (SSID)</label>
                    <input type="text" id="wifi-ssid" placeholder="Modeminizin SSID adı...">
                </div>

                <div class="input-group">
                    <label>Ağ Şifresi (SSID Password)</label>
                    <input type="password" id="wifi-pass" placeholder="Ağ şifreniz...">
                </div>

                <div style="display:flex; justify-content:space-between; gap:10px; margin-top:10px;">
                    <button class="btn btn-danger" onclick="rebootESP()">CİHAZI YENİDEN BAŞLAT</button>
                    <button class="btn btn-primary" onclick="saveWifiSettings()">KAYDET VE BAĞLAN</button>
                </div>
            </div>
        </div>
    </div>

    <footer>
        <p>ESP32 Motion Controller System © 2026. Made with ❤️ for Industrial Automation.</p>
    </footer>

    <script>
        let ws;
        let wsReconnectInterval = 2000;
        let activeTab = 'dashboard';
        
        // Track original targets & start positions for progress calculation
        let m1StartPos = 0, m1TargetPos = 0;
        let m2StartPos = 0, m2TargetPos = 0;

        function initWebSocket() {
            const host = window.location.hostname || "192.168.4.1";
            const wsUri = "ws://" + host + "/ws";
            
            console.log("WebSocket connecting to " + wsUri);
            ws = new WebSocket(wsUri);

            ws.onopen = function() {
                console.log("WebSocket connected.");
                updateWsStatus(true, "WebSocket Bağlandı");
                logConsole("WebSocket bağlantısı sağlandı.");
            };

            ws.onclose = function() {
                console.log("WebSocket disconnected.");
                updateWsStatus(false, "WebSocket Bağlantı Koptu");
                logConsole("WebSocket bağlantısı koptu. Yeniden deneniyor...", "warn");
                setTimeout(initWebSocket, wsReconnectInterval);
            };

            ws.onerror = function(err) {
                console.error("WebSocket error:", err);
                updateWsStatus(false, "WebSocket Hatası");
            };

            ws.onmessage = function(evt) {
                try {
                    const data = JSON.parse(evt.data);
                    
                    // Update variables
                    if (data.m1Pos !== undefined) {
                        document.getElementById("m1-pos-pulse").innerText = data.m1Pos;
                        document.getElementById("m1-pos-val").innerText = Number(data.m1PosU).toFixed(2);
                        document.getElementById("m1-pos-unit").innerText = getUnitName(data.m1Unit);
                    }
                    if (data.m2Pos !== undefined) {
                        document.getElementById("m2-pos-pulse").innerText = data.m2Pos;
                        document.getElementById("m2-pos-val").innerText = Number(data.m2PosU).toFixed(2);
                        document.getElementById("m2-pos-unit").innerText = getUnitName(data.m2Unit);
                    }

                    if (data.m1Target !== undefined) {
                        document.getElementById("m1-target-pulse").innerText = data.m1Target;
                        document.getElementById("m1-target-val").innerText = Number(data.m1TargetU).toFixed(2);
                        document.getElementById("m1-target-unit").innerText = getUnitName(data.m1Unit);
                        m1TargetPos = data.m1Target;
                    }
                    if (data.m2Target !== undefined) {
                        document.getElementById("m2-target-pulse").innerText = data.m2Target;
                        document.getElementById("m2-target-val").innerText = Number(data.m2TargetU).toFixed(2);
                        document.getElementById("m2-target-unit").innerText = getUnitName(data.m2Unit);
                        m2TargetPos = data.m2Target;
                    }

                    if (data.m1Dist !== undefined) {
                        document.getElementById("m1-dist-val").innerText = Number(data.m1DistU).toFixed(2);
                        document.getElementById("m1-dist-unit").innerText = getUnitName(data.m1Unit);
                    }
                    if (data.m2Dist !== undefined) {
                        document.getElementById("m2-dist-val").innerText = Number(data.m2DistU).toFixed(2);
                        document.getElementById("m2-dist-unit").innerText = getUnitName(data.m2Unit);
                    }

                    if (data.m1Speed !== undefined) document.getElementById("m1-speed").innerText = Math.round(data.m1Speed);
                    if (data.m1Accel !== undefined) document.getElementById("m1-accel").innerText = Math.round(data.m1Accel);
                    if (data.m2Speed !== undefined) document.getElementById("m2-speed").innerText = Math.round(data.m2Speed);
                    if (data.m2Accel !== undefined) document.getElementById("m2-accel").innerText = Math.round(data.m2Accel);

                    // Indicators
                    updateIndicator("m1-sw", data.m1Sw);
                    updateIndicator("m2-sw", data.m2Sw);
                    updateIndicator("m1-en", data.m1En);
                    updateIndicator("m2-en", data.m2En);

                    // Home Ok Status
                    const m1HomeOkText = document.getElementById("m1-home-ok");
                    if (data.m1HomeOk) {
                        m1HomeOkText.innerText = "Tamamlandı";
                        m1HomeOkText.style.color = "var(--success)";
                    } else {
                        m1HomeOkText.innerText = "Bulunmadı";
                        m1HomeOkText.style.color = "var(--danger)";
                    }

                    const m2HomeOkText = document.getElementById("m2-home-ok");
                    if (data.m2HomeOk) {
                        m2HomeOkText.innerText = "Tamamlandı";
                        m2HomeOkText.style.color = "var(--success)";
                    } else {
                        m2HomeOkText.innerText = "Bulunmadı";
                        m2HomeOkText.style.color = "var(--danger)";
                    }

                    // Progress percentages
                    if (data.m1Percent !== undefined) {
                        document.getElementById("m1-progress-pct").innerText = data.m1Percent + "%";
                        document.getElementById("m1-progress-bar").style.width = data.m1Percent + "%";
                    }
                    if (data.m2Percent !== undefined) {
                        document.getElementById("m2-progress-pct").innerText = data.m2Percent + "%";
                        document.getElementById("m2-progress-bar").style.width = data.m2Percent + "%";
                    }

                    // Motor statuses
                    setMotorStatusBadge(1, data.m1Status);
                    setMotorStatusBadge(2, data.m2Status);

                    // Telemetry & Uptime
                    if (data.wifiMode !== undefined) document.getElementById("sys-wifi-mode").innerText = data.wifiMode;
                    if (data.wifiIP !== undefined) document.getElementById("sys-wifi-ip").innerText = data.wifiIP;
                    if (data.uptime !== undefined) document.getElementById("sys-uptime").innerText = formatUptime(data.uptime);
                    if (data.heap !== undefined) document.getElementById("sys-heap").innerText = Math.round(data.heap / 1024) + " KB";
                    if (data.wdt !== undefined) {
                        const wdtText = document.getElementById("sys-wdt");
                        wdtText.innerText = data.wdt ? "AKTİF" : "PASİF";
                        wdtText.style.color = data.wdt ? "var(--success)" : "var(--danger)";
                    }

                    // Handle warnings/logs
                    if (data.log) {
                        const isWarn = data.log.includes("Hata") || data.log.includes("limit") || data.log.includes("Fail");
                        logConsole(data.log, isWarn ? "warn" : "info");
                    }

                    // Fill Configuration fields if received (only once on load)
                    if (data.configUpdate && activeTab === 'settings') {
                        // Avoid overwriting user's typing, only populate if input fields are untouched
                        populateConfigFields(data);
                    }

                } catch (e) {
                    console.error("JSON parse error:", e);
                }
            };
        }

        function populateConfigFields(data) {
            if (!document.activeElement || document.activeElement.tagName !== 'INPUT') {
                if (data.m1Cfg) {
                    document.getElementById("m1-cfg-speed").value = data.m1Cfg.maxSpeed;
                    document.getElementById("m1-cfg-accel").value = data.m1Cfg.acceleration;
                    document.getElementById("m1-cfg-min").value = data.m1Cfg.minPosition;
                    document.getElementById("m1-cfg-max").value = data.m1Cfg.maxPosition;
                    document.getElementById("m1-cfg-angle").value = data.m1Cfg.stepAngle;
                    document.getElementById("m1-cfg-usteps").value = data.m1Cfg.microsteps;
                    document.getElementById("m1-cfg-drive").value = data.m1Cfg.driveType;
                    document.getElementById("m1-cfg-pitch").value = data.m1Cfg.leadScrewPitch;
                    document.getElementById("m1-cfg-beltpitch").value = data.m1Cfg.beltPitch;
                    document.getElementById("m1-cfg-teeth").value = data.m1Cfg.pulleyTeeth;
                    document.getElementById("m1-cfg-unit").value = data.m1Cfg.unitType;
                    toggleDriveInputs(1);
                }
                if (data.m2Cfg) {
                    document.getElementById("m2-cfg-speed").value = data.m2Cfg.maxSpeed;
                    document.getElementById("m2-cfg-accel").value = data.m2Cfg.acceleration;
                    document.getElementById("m2-cfg-min").value = data.m2Cfg.minPosition;
                    document.getElementById("m2-cfg-max").value = data.m2Cfg.maxPosition;
                    document.getElementById("m2-cfg-angle").value = data.m2Cfg.stepAngle;
                    document.getElementById("m2-cfg-usteps").value = data.m2Cfg.microsteps;
                    document.getElementById("m2-cfg-drive").value = data.m2Cfg.driveType;
                    document.getElementById("m2-cfg-pitch").value = data.m2Cfg.leadScrewPitch;
                    document.getElementById("m2-cfg-beltpitch").value = data.m2Cfg.beltPitch;
                    document.getElementById("m2-cfg-teeth").value = data.m2Cfg.pulleyTeeth;
                    document.getElementById("m2-cfg-unit").value = data.m2Cfg.unitType;
                    toggleDriveInputs(2);
                }
                if (data.sysCfg) {
                    document.getElementById("sys-cfg-timeout-active").value = data.sysCfg.timeoutActive ? "1" : "0";
                    document.getElementById("sys-cfg-timeout-duration").value = data.sysCfg.timeoutDuration;
                }
            }
        }

        function updateWsStatus(connected, text) {
            const dot = document.getElementById("ws-status-dot");
            const label = document.getElementById("ws-status-text");
            if (connected) {
                dot.classList.add("connected");
            } else {
                dot.classList.remove("connected");
            }
            label.innerText = text;
        }

        function updateIndicator(id, active) {
            const el = document.getElementById(id);
            if (active) {
                el.classList.add("active");
            } else {
                el.classList.remove("active");
            }
        }

        function getUnitName(val) {
            switch(val) {
                case 1: return "mm";
                case 2: return "cm";
                case 3: return "deg";
                case 0:
                default: return "pulse";
            }
        }

        function setMotorStatusBadge(motorId, status) {
            const badge = document.getElementById(`m${motorId}-status-badge`);
            badge.className = "badge"; // Reset classes
            
            // Statuses: 0: Disabled/Pasif, 1: Idle (Ready), 2: Moving, 3: Homing, 4: Error/Failed
            switch(status) {
                case 0:
                    badge.innerText = "Pasif";
                    badge.classList.add("badge-disabled");
                    break;
                case 1:
                    badge.innerText = "Hazır";
                    badge.classList.add("badge-idle");
                    break;
                case 2:
                    badge.innerText = "Hareketli";
                    badge.classList.add("badge-moving");
                    break;
                case 3:
                    badge.innerText = "Homing";
                    badge.classList.add("badge-homing");
                    break;
                case 4:
                default:
                    badge.innerText = "Hata";
                    badge.classList.add("badge-error");
                    break;
            }
        }

        function formatUptime(seconds) {
            const hrs = Math.floor(seconds / 3600);
            const mins = Math.floor((seconds % 3600) / 60);
            const secs = seconds % 60;
            return `${hrs.toString().padStart(2, '0')}:${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
        }

        function logConsole(message, type = "info") {
            const consoleEl = document.getElementById("sys-console");
            const now = new Date();
            const timeStr = `[${now.getHours().toString().padStart(2, '0')}:${now.getMinutes().toString().padStart(2, '0')}:${now.getSeconds().toString().padStart(2, '0')}]`;
            
            let colorClass = "";
            if (type === "warn") colorClass = "console-warn";
            else if (type === "error") colorClass = "console-err";

            consoleEl.innerHTML += `
                <div class="console-entry ${colorClass}">
                    <span class="console-time">${timeStr}</span>
                    <span class="console-text">${message}</span>
                </div>
            `;
            consoleEl.scrollTop = consoleEl.scrollHeight;
        }

        // TAB NAVIGATION
        function switchTab(tabId) {
            activeTab = tabId;
            document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
            document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));
            
            event.currentTarget.classList.add('active');
            document.getElementById(`tab-${tabId}`).classList.add('active');

            if (tabId === 'settings') {
                // Request configuration details specifically from device
                sendWsMsg({ cmd: "req_config" });
            }
        }

        function toggleDriveInputs(motorId) {
            const val = document.getElementById(`m${motorId}-cfg-drive`).value;
            if (val === "0") {
                document.getElementById(`m${motorId}-drive-screw`).style.display = "block";
                document.getElementById(`m${motorId}-drive-belt`).style.display = "none";
            } else {
                document.getElementById(`m${motorId}-drive-screw`).style.display = "none";
                document.getElementById(`m${motorId}-drive-belt`).style.display = "grid";
            }
        }

        // MOTOR ACTIONS
        function sendWsMsg(obj) {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify(obj));
            } else {
                logConsole("İşlem Başarısız: WebSocket bağlı değil!", "error");
            }
        }

        function moveMotor(motorId) {
            const input = document.getElementById(`m${motorId}-goto-input`);
            const val = parseFloat(input.value);
            if (isNaN(val)) {
                logConsole(`Hata: Motor ${motorId} için geçersiz pozisyon değeri.`, "error");
                return;
            }
            logConsole(`Motor ${motorId} konumlandırılıyor: ${val}`);
            sendWsMsg({
                cmd: "move",
                motor: motorId,
                val: val
            });
        }

        function jogMotor(motorId, amount) {
            logConsole(`Motor ${motorId} jog hareketi: ${amount}`);
            sendWsMsg({
                cmd: "jog",
                motor: motorId,
                val: amount
            });
        }

        function stopMotor(motorId) {
            logConsole(`Motor ${motorId} durduruluyor...`);
            sendWsMsg({
                cmd: "stop",
                motor: motorId
            });
        }

        function homeMotor(motorId) {
            logConsole(`Motor ${motorId} homing çevrimi başlatılıyor...`);
            sendWsMsg({
                cmd: "home",
                motor: motorId
            });
        }

        function resetMotorPos(motorId) {
            logConsole(`Motor ${motorId} pozisyonu sıfırlanıyor.`);
            sendWsMsg({
                cmd: "reset",
                motor: motorId
            });
        }

        function enableMotor(motorId, state) {
            logConsole(`Motor ${motorId} driver ${state ? 'aktif' : 'pasif'} ediliyor.`);
            sendWsMsg({
                cmd: "enable",
                motor: motorId,
                val: state ? 1 : 0
            });
        }

        function sendAllStop() {
            logConsole("ACİL DURDURMA ALINDI! Tüm motorlar durduruluyor.", "error");
            sendWsMsg({
                cmd: "allstop"
            });
        }

        function rebootESP() {
            if (confirm("ESP32 cihazını yeniden başlatmak istediğinize emin misiniz?")) {
                logConsole("Sistem yeniden başlatılıyor...", "warn");
                sendWsMsg({ cmd: "reboot" });
            }
        }

        // SAVE CONFIGURATIONS
        function saveMotorSettings() {
            const cfg = {
                cmd: "save_config",
                m1: {
                    maxSpeed: parseFloat(document.getElementById("m1-cfg-speed").value),
                    acceleration: parseFloat(document.getElementById("m1-cfg-accel").value),
                    minPosition: parseInt(document.getElementById("m1-cfg-min").value),
                    maxPosition: parseInt(document.getElementById("m1-cfg-max").value),
                    stepAngle: parseFloat(document.getElementById("m1-cfg-angle").value),
                    microsteps: parseInt(document.getElementById("m1-cfg-usteps").value),
                    driveType: parseInt(document.getElementById("m1-cfg-drive").value),
                    leadScrewPitch: parseFloat(document.getElementById("m1-cfg-pitch").value),
                    beltPitch: parseFloat(document.getElementById("m1-cfg-beltpitch").value),
                    pulleyTeeth: parseInt(document.getElementById("m1-cfg-teeth").value),
                    unitType: parseInt(document.getElementById("m1-cfg-unit").value)
                },
                m2: {
                    maxSpeed: parseFloat(document.getElementById("m2-cfg-speed").value),
                    acceleration: parseFloat(document.getElementById("m2-cfg-accel").value),
                    minPosition: parseInt(document.getElementById("m2-cfg-min").value),
                    maxPosition: parseInt(document.getElementById("m2-cfg-max").value),
                    stepAngle: parseFloat(document.getElementById("m2-cfg-angle").value),
                    microsteps: parseInt(document.getElementById("m2-cfg-usteps").value),
                    driveType: parseInt(document.getElementById("m2-cfg-drive").value),
                    leadScrewPitch: parseFloat(document.getElementById("m2-cfg-pitch").value),
                    beltPitch: parseFloat(document.getElementById("m2-cfg-beltpitch").value),
                    pulleyTeeth: parseInt(document.getElementById("m2-cfg-teeth").value),
                    unitType: parseInt(document.getElementById("m2-cfg-unit").value)
                },
                sys: {
                    timeoutActive: document.getElementById("sys-cfg-timeout-active").value === "1",
                    timeoutDuration: parseInt(document.getElementById("sys-cfg-timeout-duration").value)
                }
            };
            
            logConsole("Motor mekanik ayarları gönderiliyor...");
            sendWsMsg(cfg);
        }

        function saveWifiSettings() {
            const ssid = document.getElementById("wifi-ssid").value;
            const pass = document.getElementById("wifi-pass").value;

            if (ssid.length === 0) {
                alert("Wi-Fi SSID adı boş bırakılamaz!");
                return;
            }

            const wifiCfg = {
                cmd: "save_wifi",
                ssid: ssid,
                pass: pass
            };

            logConsole("Wi-Fi ayarları kaydediliyor, ESP32 yeniden bağlanacak...");
            sendWsMsg(wifiCfg);
        }

        // Initialize on load
        window.addEventListener('load', () => {
            initWebSocket();
        });
    </script>
</body>
</html>
)rawhtml";

#endif // INDEX_HTML_H
