#ifndef INDEX_HTML_H
#define INDEX_HTML_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Dual Axis Motion Control</title>
    <style>
        :root {
            --bg: #0b0f19;
            --panel: rgba(20, 30, 55, 0.72);
            --panel-2: rgba(255, 255, 255, 0.04);
            --border: rgba(255, 255, 255, 0.1);
            --text: #f3f4f6;
            --muted: #9ca3af;
            --primary: #3b82f6;
            --success: #10b981;
            --danger: #ef4444;
            --warning: #f59e0b;
            --accent: #8b5cf6;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            font-family: Inter, Segoe UI, Arial, sans-serif;
        }

        body {
            min-height: 100vh;
            background:
                radial-gradient(circle at top left, rgba(59, 130, 246, 0.16), transparent 32rem),
                radial-gradient(circle at bottom right, rgba(16, 185, 129, 0.12), transparent 30rem),
                var(--bg);
            color: var(--text);
            padding: 20px;
        }

        header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 16px;
            padding-bottom: 18px;
            border-bottom: 1px solid var(--border);
            margin-bottom: 18px;
            flex-wrap: wrap;
        }

        h1 {
            font-size: 1.45rem;
            font-weight: 750;
        }

        h2 {
            font-size: 1.05rem;
            font-weight: 700;
        }

        h3 {
            font-size: 0.95rem;
            font-weight: 700;
        }

        .header-actions {
            display: flex;
            align-items: center;
            gap: 10px;
            flex-wrap: wrap;
        }

        .status-badge {
            display: inline-flex;
            align-items: center;
            gap: 8px;
            padding: 7px 12px;
            border-radius: 999px;
            border: 1px solid var(--border);
            background: var(--panel-2);
            color: var(--muted);
            font-size: 0.86rem;
            font-weight: 600;
        }

        .status-dot,
        .indicator {
            width: 10px;
            height: 10px;
            border-radius: 50%;
            display: inline-block;
            background: rgba(255, 255, 255, 0.18);
        }

        .status-dot.connected,
        .indicator.active {
            background: var(--success);
            box-shadow: 0 0 10px rgba(16, 185, 129, 0.5);
        }

        .tabs {
            display: flex;
            gap: 10px;
            padding-bottom: 14px;
            margin-bottom: 18px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.06);
            overflow-x: auto;
        }

        .tab {
            padding: 10px 16px;
            border-radius: 8px;
            border: 1px solid var(--border);
            color: var(--muted);
            background: rgba(255, 255, 255, 0.03);
            cursor: pointer;
            white-space: nowrap;
            font-weight: 650;
        }

        .tab.active {
            background: var(--primary);
            color: white;
            border-color: transparent;
        }

        .tab-content {
            display: none;
        }

        .tab-content.active {
            display: block;
        }

        .dashboard-grid {
            display: grid;
            grid-template-columns: repeat(3, minmax(300px, 1fr));
            align-items: stretch;
            gap: 18px;
        }

        .settings-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(320px, 1fr));
            gap: 18px;
        }

        .card {
            background: var(--panel);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 18px;
            display: flex;
            flex-direction: column;
            gap: 14px;
            backdrop-filter: blur(14px);
        }

        .wide-card {
            grid-column: 1 / -1;
        }

        .card-title {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 12px;
            padding-bottom: 10px;
            border-bottom: 1px solid rgba(255, 255, 255, 0.06);
        }

        .subtitle {
            color: var(--muted);
            font-size: 0.82rem;
            margin-top: 4px;
        }

        .stat-grid {
            display: grid;
            grid-template-columns: repeat(2, minmax(0, 1fr));
            gap: 10px;
        }

        .stat-box {
            background: var(--panel-2);
            border: 1px solid rgba(255, 255, 255, 0.06);
            border-radius: 8px;
            padding: 10px;
            min-width: 0;
        }

        .stat-label {
            display: block;
            color: var(--muted);
            font-size: 0.72rem;
            text-transform: uppercase;
            margin-bottom: 4px;
        }

        .stat-value {
            font-size: 1.08rem;
            font-weight: 750;
            overflow-wrap: anywhere;
        }

        .stat-unit {
            color: var(--muted);
            font-size: 0.78rem;
            margin-left: 4px;
            font-weight: 500;
        }

        .badge {
            padding: 4px 8px;
            border-radius: 6px;
            font-size: 0.72rem;
            font-weight: 800;
            text-transform: uppercase;
        }

        .badge-disabled { background: rgba(156, 163, 175, 0.16); color: #d1d5db; }
        .badge-idle { background: rgba(59, 130, 246, 0.16); color: #93c5fd; }
        .badge-moving { background: rgba(16, 185, 129, 0.16); color: #6ee7b7; }
        .badge-homing { background: rgba(245, 158, 11, 0.16); color: #fcd34d; }
        .badge-error { background: rgba(239, 68, 68, 0.16); color: #fca5a5; }

        .progress-container {
            height: 10px;
            overflow: hidden;
            border-radius: 999px;
            background: rgba(255, 255, 255, 0.08);
        }

        .progress-bar {
            height: 100%;
            width: 0%;
            border-radius: 999px;
            background: linear-gradient(90deg, var(--primary), var(--accent));
            transition: width 0.25s ease;
        }

        .input-group {
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        label {
            color: var(--muted);
            font-size: 0.8rem;
            font-weight: 650;
        }

        input,
        select,
        textarea {
            width: 100%;
            background: rgba(255, 255, 255, 0.05);
            color: var(--text);
            border: 1px solid rgba(255, 255, 255, 0.1);
            border-radius: 8px;
            padding: 10px 11px;
            outline: none;
            font-size: 0.93rem;
        }

        textarea {
            min-height: 300px;
            resize: vertical;
            font-family: Consolas, monospace;
            line-height: 1.45;
        }

        input:focus,
        select:focus,
        textarea:focus {
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(59, 130, 246, 0.25);
        }

        .input-with-btn,
        .button-row,
        .motor-controls {
            display: grid;
            grid-template-columns: repeat(2, minmax(0, 1fr));
            gap: 9px;
        }

        .jog-grid {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 9px;
        }

        .relay-grid {
            display: grid;
            grid-template-columns: 1fr;
            gap: 14px;
        }

        .relay-card .stat-grid {
            grid-template-columns: 1fr;
        }

        .program-grid {
            display: grid;
            grid-template-columns: minmax(360px, 1.35fr) minmax(300px, 0.8fr);
            gap: 18px;
            align-items: start;
        }

        .program-status {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 10px;
        }

        .command-list {
            display: grid;
            gap: 8px;
            color: var(--muted);
            font-size: 0.84rem;
            line-height: 1.45;
        }

        .command-list code {
            color: #bfdbfe;
            font-family: Consolas, monospace;
        }

        .btn,
        .emergency-btn {
            min-height: 40px;
            border: 1px solid rgba(255, 255, 255, 0.12);
            border-radius: 8px;
            color: white;
            background: rgba(255, 255, 255, 0.08);
            cursor: pointer;
            font-weight: 800;
            font-size: 0.88rem;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            gap: 6px;
            padding: 9px 12px;
        }

        .btn:hover,
        .emergency-btn:hover {
            filter: brightness(1.12);
        }

        .btn-primary { background: #2563eb; border-color: transparent; }
        .btn-success { background: #059669; border-color: transparent; }
        .btn-warning { background: #d97706; border-color: transparent; }
        .btn-danger,
        .emergency-btn { background: #dc2626; border-color: transparent; }

        .state-row {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 8px;
            color: var(--muted);
            font-size: 0.82rem;
        }

        .console {
            height: 160px;
            overflow: auto;
            background: #050914;
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 12px;
            font-family: Consolas, monospace;
            font-size: 0.82rem;
            line-height: 1.45;
            color: #86efac;
        }

        .console-entry {
            display: flex;
            gap: 8px;
        }

        .console-time {
            color: #93c5fd;
            flex: 0 0 auto;
        }

        .console-warn { color: var(--warning); }
        .console-err { color: #fca5a5; }

        footer {
            padding: 24px 0 4px;
            text-align: center;
            color: var(--muted);
            font-size: 0.78rem;
        }

        @media (max-width: 1080px) {
            .dashboard-grid {
                grid-template-columns: 1fr;
            }
            .program-grid {
                grid-template-columns: 1fr;
            }
        }

        @media (max-width: 720px) {
            body { padding: 12px; }
            header { align-items: stretch; }
            .header-actions,
            .button-row,
            .motor-controls,
            .input-with-btn {
                grid-template-columns: 1fr;
                display: grid;
                width: 100%;
            }
            .stat-grid,
            .state-row,
            .program-status {
                grid-template-columns: 1fr;
            }
        }
    </style>
</head>
<body>
    <header>
        <div>
            <h1>ESP32 Dual Axis Motion Control</h1>
            <div class="subtitle">Two stepper motors, homing switches, timed general outputs, and live telemetry.</div>
        </div>
        <div class="header-actions">
            <span class="status-badge">
                <span id="ws-status-dot" class="status-dot"></span>
                <span id="ws-status-text">WebSocket: disconnected</span>
            </span>
            <button class="emergency-btn" onclick="sendAllStop()">All Stop</button>
        </div>
    </header>

    <nav class="tabs">
        <button class="tab active" onclick="switchTab(event, 'dashboard')">Dashboard</button>
        <button class="tab" onclick="switchTab(event, 'settings')">Motor Settings</button>
        <button class="tab" onclick="switchTab(event, 'program')">Program</button>
        <button class="tab" onclick="switchTab(event, 'wifi-tab')">Wi-Fi Settings</button>
    </nav>

    <section id="tab-dashboard" class="tab-content active">
        <div class="dashboard-grid">
            <article class="card">
                <div class="card-title">
                    <div>
                        <h2>X Axis - Motor 1</h2>
                        <div class="subtitle">STEP GPIO25, DIR GPIO26, ENABLE GPIO27, HOME GPIO32</div>
                    </div>
                    <span id="m1-status-badge" class="badge badge-disabled">Disabled</span>
                </div>

                <div class="stat-grid">
                    <div class="stat-box">
                        <span class="stat-label">Current Position</span>
                        <span class="stat-value"><span id="m1-pos-val">0.00</span><span id="m1-pos-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label">Pulse: <span id="m1-pos-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Target Position</span>
                        <span class="stat-value"><span id="m1-target-val">0.00</span><span id="m1-target-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label">Pulse: <span id="m1-target-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Remaining Distance</span>
                        <span class="stat-value"><span id="m1-dist-val">0.00</span><span id="m1-dist-unit" class="stat-unit">mm</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Speed / Acceleration</span>
                        <span class="stat-value" style="font-size:0.94rem">Speed: <span id="m1-speed">0</span> Hz<br>Accel: <span id="m1-accel">0</span> Hz/s2</span>
                    </div>
                </div>

                <div style="display:flex;justify-content:space-between;color:var(--muted);font-size:0.82rem">
                    <span>Progress</span>
                    <span id="m1-progress-pct">100%</span>
                </div>
                <div class="progress-container"><div id="m1-progress-bar" class="progress-bar"></div></div>

                <div class="state-row">
                    <div>Home Switch: <span id="m1-sw" class="indicator"></span></div>
                    <div>Driver Enable: <span id="m1-en" class="indicator"></span></div>
                    <div>Home: <span id="m1-home-ok" style="font-weight:800;color:var(--danger)">Not Set</span></div>
                </div>

                <div class="input-group">
                    <label for="m1-goto-input">Move To Position</label>
                    <div class="input-with-btn">
                        <input type="number" id="m1-goto-input" step="0.01" placeholder="Position">
                        <button class="btn btn-primary" onclick="moveMotor(1)">Go</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button class="btn btn-warning" onclick="jogMotor(1, -10)">-10</button>
                    <button class="btn btn-danger" onclick="stopMotor(1)">Stop</button>
                    <button class="btn btn-success" onclick="jogMotor(1, 10)">+10</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-primary" onclick="homeMotor(1)">Home</button>
                    <button class="btn" onclick="resetMotorPos(1)">Set Zero</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-success" onclick="enableMotor(1, true)">Enable Driver</button>
                    <button class="btn btn-danger" onclick="enableMotor(1, false)">Disable Driver</button>
                </div>
            </article>

            <article class="card">
                <div class="card-title">
                    <div>
                        <h2>Y Axis - Motor 2</h2>
                        <div class="subtitle">STEP GPIO18, DIR GPIO19, ENABLE GPIO21, HOME GPIO33</div>
                    </div>
                    <span id="m2-status-badge" class="badge badge-disabled">Disabled</span>
                </div>

                <div class="stat-grid">
                    <div class="stat-box">
                        <span class="stat-label">Current Position</span>
                        <span class="stat-value"><span id="m2-pos-val">0.00</span><span id="m2-pos-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label">Pulse: <span id="m2-pos-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Target Position</span>
                        <span class="stat-value"><span id="m2-target-val">0.00</span><span id="m2-target-unit" class="stat-unit">mm</span></span>
                        <span class="stat-label">Pulse: <span id="m2-target-pulse">0</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Remaining Distance</span>
                        <span class="stat-value"><span id="m2-dist-val">0.00</span><span id="m2-dist-unit" class="stat-unit">mm</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Speed / Acceleration</span>
                        <span class="stat-value" style="font-size:0.94rem">Speed: <span id="m2-speed">0</span> Hz<br>Accel: <span id="m2-accel">0</span> Hz/s2</span>
                    </div>
                </div>

                <div style="display:flex;justify-content:space-between;color:var(--muted);font-size:0.82rem">
                    <span>Progress</span>
                    <span id="m2-progress-pct">100%</span>
                </div>
                <div class="progress-container"><div id="m2-progress-bar" class="progress-bar"></div></div>

                <div class="state-row">
                    <div>Home Switch: <span id="m2-sw" class="indicator"></span></div>
                    <div>Driver Enable: <span id="m2-en" class="indicator"></span></div>
                    <div>Home: <span id="m2-home-ok" style="font-weight:800;color:var(--danger)">Not Set</span></div>
                </div>

                <div class="input-group">
                    <label for="m2-goto-input">Move To Position</label>
                    <div class="input-with-btn">
                        <input type="number" id="m2-goto-input" step="0.01" placeholder="Position">
                        <button class="btn btn-primary" onclick="moveMotor(2)">Go</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button class="btn btn-warning" onclick="jogMotor(2, -10)">-10</button>
                    <button class="btn btn-danger" onclick="stopMotor(2)">Stop</button>
                    <button class="btn btn-success" onclick="jogMotor(2, 10)">+10</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-primary" onclick="homeMotor(2)">Home</button>
                    <button class="btn" onclick="resetMotorPos(2)">Set Zero</button>
                </div>

                <div class="motor-controls">
                    <button class="btn btn-success" onclick="enableMotor(2, true)">Enable Driver</button>
                    <button class="btn btn-danger" onclick="enableMotor(2, false)">Disable Driver</button>
                </div>
            </article>

            <article class="card relay-card">
                <div class="card-title">
                    <div>
                        <h2>Timed General Outputs</h2>
                        <div class="subtitle">Gen.Out1 uses GPIO23. Gen.Out2 uses GPIO5. The output turns off automatically when the timer expires.</div>
                    </div>
                </div>
                <div class="relay-grid">
                    <div class="stat-box">
                        <div class="card-title" style="border:0;padding:0;margin-bottom:8px">
                            <h3>Gen.Out1</h3>
                            <span class="status-badge"><span id="r1-ind" class="indicator"></span><span id="r1-state">Off</span></span>
                        </div>
                        <div class="stat-grid">
                            <div class="input-group">
                                <label for="r1-duration">On Time (seconds)</label>
                                <input type="number" id="r1-duration" min="1" max="86400" value="5">
                            </div>
                            <div class="stat-box">
                                <span class="stat-label">Remaining</span>
                                <span class="stat-value"><span id="r1-remaining">0</span><span class="stat-unit">s</span></span>
                            </div>
                        </div>
                        <div class="button-row" style="margin-top:10px">
                            <button class="btn btn-success" onclick="triggerRelay(1)">Trigger Gen.Out1</button>
                            <button class="btn btn-danger" onclick="turnRelayOff(1)">Turn Off</button>
                        </div>
                    </div>

                    <div class="stat-box">
                        <div class="card-title" style="border:0;padding:0;margin-bottom:8px">
                            <h3>Gen.Out2</h3>
                            <span class="status-badge"><span id="r2-ind" class="indicator"></span><span id="r2-state">Off</span></span>
                        </div>
                        <div class="stat-grid">
                            <div class="input-group">
                                <label for="r2-duration">On Time (seconds)</label>
                                <input type="number" id="r2-duration" min="1" max="86400" value="5">
                            </div>
                            <div class="stat-box">
                                <span class="stat-label">Remaining</span>
                                <span class="stat-value"><span id="r2-remaining">0</span><span class="stat-unit">s</span></span>
                            </div>
                        </div>
                        <div class="button-row" style="margin-top:10px">
                            <button class="btn btn-success" onclick="triggerRelay(2)">Trigger Gen.Out2</button>
                            <button class="btn btn-danger" onclick="turnRelayOff(2)">Turn Off</button>
                        </div>
                    </div>
                </div>
            </article>

            <article class="card wide-card">
                <div class="card-title">
                    <h2>System Information</h2>
                    <span class="status-badge">WDT: <b id="sys-wdt" style="color:var(--success)">Active</b></span>
                </div>
                <div class="stat-grid">
                    <div class="stat-box"><span class="stat-label">Connection Mode</span><span class="stat-value" id="sys-wifi-mode">-</span></div>
                    <div class="stat-box"><span class="stat-label">System IP Address</span><span class="stat-value" id="sys-wifi-ip">-</span></div>
                    <div class="stat-box"><span class="stat-label">Uptime</span><span class="stat-value" id="sys-uptime">00:00:00</span></div>
                    <div class="stat-box"><span class="stat-label">Free Heap</span><span class="stat-value" id="sys-heap">0 KB</span></div>
                </div>
                <h2>System Log</h2>
                <div class="console" id="sys-console">
                    <div class="console-entry"><span class="console-time">[00:00:00]</span><span>Interface started. Waiting for WebSocket connection...</span></div>
                </div>
                <button class="btn" onclick="document.getElementById('sys-console').innerHTML=''">Clear Log</button>
            </article>
        </div>
    </section>

    <section id="tab-settings" class="tab-content">
        <article class="card">
            <div class="card-title"><h2>Motor Mechanical And Limit Configuration</h2></div>
            <div class="settings-grid">
                <div style="display:flex;flex-direction:column;gap:12px">
                    <h3>Motor 1 - X Axis</h3>
                    <div class="stat-grid">
                        <div class="input-group"><label>Speed Limit (steps/s)</label><input type="number" id="m1-cfg-speed" value="2000"></div>
                        <div class="input-group"><label>Acceleration (steps/s2)</label><input type="number" id="m1-cfg-accel" value="1000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Software Minimum Limit (steps)</label><input type="number" id="m1-cfg-min" value="0"></div>
                        <div class="input-group"><label>Software Maximum Limit (steps)</label><input type="number" id="m1-cfg-max" value="50000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Step Angle (degrees)</label><input type="number" id="m1-cfg-angle" step="0.1" value="1.8"></div>
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
                        <label>Drive System</label>
                        <select id="m1-cfg-drive" onchange="toggleDriveInputs(1)">
                            <option value="0" selected>Lead Screw</option>
                            <option value="1">Belt And Pulley</option>
                        </select>
                    </div>
                    <div id="m1-drive-screw" class="input-group"><label>Lead Screw Pitch (mm/rev)</label><input type="number" id="m1-cfg-pitch" step="0.01" value="8.00"></div>
                    <div id="m1-drive-belt" class="stat-grid" style="display:none">
                        <div class="input-group"><label>Belt Pitch (mm)</label><input type="number" id="m1-cfg-beltpitch" step="0.01" value="2.00"></div>
                        <div class="input-group"><label>Pulley Teeth</label><input type="number" id="m1-cfg-teeth" value="20"></div>
                    </div>
                    <div class="input-group">
                        <label>Interface Unit</label>
                        <select id="m1-cfg-unit">
                            <option value="0">Pulse</option>
                            <option value="1" selected>Millimeter (mm)</option>
                            <option value="2">Centimeter (cm)</option>
                            <option value="3">Degree</option>
                        </select>
                    </div>
                </div>

                <div style="display:flex;flex-direction:column;gap:12px">
                    <h3>Motor 2 - Y Axis</h3>
                    <div class="stat-grid">
                        <div class="input-group"><label>Speed Limit (steps/s)</label><input type="number" id="m2-cfg-speed" value="2000"></div>
                        <div class="input-group"><label>Acceleration (steps/s2)</label><input type="number" id="m2-cfg-accel" value="1000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Software Minimum Limit (steps)</label><input type="number" id="m2-cfg-min" value="0"></div>
                        <div class="input-group"><label>Software Maximum Limit (steps)</label><input type="number" id="m2-cfg-max" value="100000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Step Angle (degrees)</label><input type="number" id="m2-cfg-angle" step="0.1" value="1.8"></div>
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
                        <label>Drive System</label>
                        <select id="m2-cfg-drive" onchange="toggleDriveInputs(2)">
                            <option value="0" selected>Lead Screw</option>
                            <option value="1">Belt And Pulley</option>
                        </select>
                    </div>
                    <div id="m2-drive-screw" class="input-group"><label>Lead Screw Pitch (mm/rev)</label><input type="number" id="m2-cfg-pitch" step="0.01" value="8.00"></div>
                    <div id="m2-drive-belt" class="stat-grid" style="display:none">
                        <div class="input-group"><label>Belt Pitch (mm)</label><input type="number" id="m2-cfg-beltpitch" step="0.01" value="2.00"></div>
                        <div class="input-group"><label>Pulley Teeth</label><input type="number" id="m2-cfg-teeth" value="20"></div>
                    </div>
                    <div class="input-group">
                        <label>Interface Unit</label>
                        <select id="m2-cfg-unit">
                            <option value="0">Pulse</option>
                            <option value="1" selected>Millimeter (mm)</option>
                            <option value="2">Centimeter (cm)</option>
                            <option value="3">Degree</option>
                        </select>
                    </div>
                </div>
            </div>

            <div class="stat-box">
                <h3>Driver Power Management</h3>
                <div class="stat-grid" style="margin-top:10px">
                    <div class="input-group">
                        <label>Auto-Disable Feature</label>
                        <select id="sys-cfg-timeout-active">
                            <option value="1" selected>Enabled</option>
                            <option value="0">Disabled</option>
                        </select>
                    </div>
                    <div class="input-group"><label>Timeout Duration (seconds)</label><input type="number" id="sys-cfg-timeout-duration" value="10" min="1"></div>
                </div>
            </div>
            <div style="display:flex;justify-content:flex-end">
                <button class="btn btn-primary" onclick="saveMotorSettings()">Save And Apply Settings</button>
            </div>
        </article>
    </section>

    <section id="tab-program" class="tab-content">
        <div class="program-grid">
            <article class="card">
                <div class="card-title">
                    <div>
                        <h2>Sequential Program</h2>
                        <div class="subtitle">Write one command per line. The ESP32 runs the lines in order and waits for each motor move to finish.</div>
                    </div>
                    <span id="program-run-badge" class="badge badge-idle">Idle</span>
                </div>

                <div class="program-status">
                    <div class="stat-box">
                        <span class="stat-label">Status</span>
                        <span class="stat-value" id="program-status">Stopped</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Current Line</span>
                        <span class="stat-value" id="program-line" style="font-size:0.9rem">-</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Mode</span>
                        <span class="stat-value" style="font-size:0.9rem">Relative</span>
                    </div>
                </div>

                <div class="input-group">
                    <label for="program-editor">Program Text</label>
                    <textarea id="program-editor" spellcheck="false"># X and Y values are relative movement in the active motor unit
# VX and VY values are speed in steps/s
VX 5000
VY 5000
X 50
Y 10
D 50
Gen1 ON
D 100
X -50
Gen2 OFF
D 200
Gen1 OFF</textarea>
                </div>

                <div class="button-row">
                    <button class="btn btn-success" onclick="runProgram()">Run Program</button>
                    <button class="btn btn-danger" onclick="stopProgram()">Stop Program</button>
                </div>
            </article>

            <article class="card">
                <div class="card-title">
                    <h2>Program Help</h2>
                </div>
                <div class="command-list">
                    <div><code>X 50</code> moves the X axis, Motor 1, +50 relative units.</div>
                    <div><code>X -50</code> moves the X axis, Motor 1, -50 relative units.</div>
                    <div><code>Y 10</code> moves the Y axis, Motor 2, +10 relative units.</div>
                    <div><code>VX 5000</code> sets X axis, Motor 1, speed to 5000 steps/s.</div>
                    <div><code>VY 5000</code> sets Y axis, Motor 2, speed to 5000 steps/s.</div>
                    <div><code>D 100</code> waits 100 milliseconds before the next line.</div>
                    <div><code>Gen1 ON</code> turns Gen.Out1 on until another command turns it off.</div>
                    <div><code>Gen2 OFF</code> turns Gen.Out2 off.</div>
                    <div>Use <code>#</code> or <code>//</code> for comments. Empty lines are ignored.</div>
                    <div>The movement unit is the unit selected in Motor Settings: mm, cm, degree, or pulse.</div>
                </div>
            </article>
        </div>
    </section>

    <section id="tab-wifi-tab" class="tab-content">
        <article class="card" style="max-width:620px;margin:0 auto">
            <div class="card-title"><h2>Wi-Fi Connection Settings</h2></div>
            <div class="stat-box">
                <h3>Access Point And Station Mode</h3>
                <p class="subtitle">The ESP32 always starts its own access point with SSID ESP32-MOTOR and password 12345678. If station credentials are saved, it also tries to join that local network after boot.</p>
            </div>
            <div class="input-group"><label for="wifi-ssid">Wireless Network Name (SSID)</label><input type="text" id="wifi-ssid" placeholder="Network SSID"></div>
            <div class="input-group"><label for="wifi-pass">Wireless Password</label><input type="password" id="wifi-pass" placeholder="Network password"></div>
            <div class="button-row">
                <button class="btn btn-danger" onclick="rebootESP()">Restart Device</button>
                <button class="btn btn-primary" onclick="saveWifiSettings()">Save Wi-Fi</button>
            </div>
        </article>
    </section>

    <footer>ESP32 Motion Controller System - 2026</footer>

    <script>
        let ws;
        const wsReconnectInterval = 2000;
        let activeTab = "dashboard";

        function initWebSocket() {
            const host = window.location.hostname || "192.168.4.1";
            ws = new WebSocket("ws://" + host + "/ws");

            ws.onopen = function() {
                updateWsStatus(true, "WebSocket: connected");
                logConsole("WebSocket connection established.");
            };

            ws.onclose = function() {
                updateWsStatus(false, "WebSocket: disconnected");
                logConsole("WebSocket disconnected. Reconnecting...", "warn");
                setTimeout(initWebSocket, wsReconnectInterval);
            };

            ws.onerror = function() {
                updateWsStatus(false, "WebSocket: error");
            };

            ws.onmessage = function(evt) {
                try {
                    const data = JSON.parse(evt.data);
                    if (data.configUpdate) {
                        if (activeTab === "settings") populateConfigFields(data);
                        return;
                    }
                    updateTelemetry(data);
                } catch (e) {
                    console.error("JSON parse error", e);
                }
            };
        }

        function updateTelemetry(data) {
            updateMotorTelemetry(1, data);
            updateMotorTelemetry(2, data);

            if (data.wifiMode !== undefined) document.getElementById("sys-wifi-mode").innerText = data.wifiMode;
            if (data.wifiIP !== undefined) document.getElementById("sys-wifi-ip").innerText = data.wifiIP;
            if (data.uptime !== undefined) document.getElementById("sys-uptime").innerText = formatUptime(data.uptime);
            if (data.heap !== undefined) document.getElementById("sys-heap").innerText = Math.round(data.heap / 1024) + " KB";
            if (data.wdt !== undefined) {
                const wdt = document.getElementById("sys-wdt");
                wdt.innerText = data.wdt ? "Active" : "Inactive";
                wdt.style.color = data.wdt ? "var(--success)" : "var(--danger)";
            }

            updateRelayTelemetry(1, data.r1Active, data.r1Remaining);
            updateRelayTelemetry(2, data.r2Active, data.r2Remaining);
            updateProgramTelemetry(data);

            if (data.log) {
                const isWarn = data.log.includes("Error") || data.log.includes("limit") || data.log.includes("Emergency");
                logConsole(data.log, isWarn ? "warn" : "info");
            }
        }

        function updateMotorTelemetry(motorId, data) {
            const prefix = "m" + motorId;
            const pos = data[prefix + "Pos"];
            const unit = data[prefix + "Unit"];
            if (pos !== undefined) {
                document.getElementById(prefix + "-pos-pulse").innerText = pos;
                document.getElementById(prefix + "-pos-val").innerText = Number(data[prefix + "PosU"]).toFixed(2);
                document.getElementById(prefix + "-pos-unit").innerText = getUnitName(unit);
            }
            if (data[prefix + "Target"] !== undefined) {
                document.getElementById(prefix + "-target-pulse").innerText = data[prefix + "Target"];
                document.getElementById(prefix + "-target-val").innerText = Number(data[prefix + "TargetU"]).toFixed(2);
                document.getElementById(prefix + "-target-unit").innerText = getUnitName(unit);
            }
            if (data[prefix + "Dist"] !== undefined) {
                document.getElementById(prefix + "-dist-val").innerText = Number(data[prefix + "DistU"]).toFixed(2);
                document.getElementById(prefix + "-dist-unit").innerText = getUnitName(unit);
            }
            if (data[prefix + "Speed"] !== undefined) document.getElementById(prefix + "-speed").innerText = Math.round(data[prefix + "Speed"]);
            if (data[prefix + "Accel"] !== undefined) document.getElementById(prefix + "-accel").innerText = Math.round(data[prefix + "Accel"]);

            updateIndicator(prefix + "-sw", data[prefix + "Sw"]);
            updateIndicator(prefix + "-en", data[prefix + "En"]);

            const home = document.getElementById(prefix + "-home-ok");
            if (data[prefix + "HomeOk"]) {
                home.innerText = "Complete";
                home.style.color = "var(--success)";
            } else {
                home.innerText = "Not Set";
                home.style.color = "var(--danger)";
            }

            if (data[prefix + "Percent"] !== undefined) {
                document.getElementById(prefix + "-progress-pct").innerText = data[prefix + "Percent"] + "%";
                document.getElementById(prefix + "-progress-bar").style.width = data[prefix + "Percent"] + "%";
            }
            setMotorStatusBadge(motorId, data[prefix + "Status"]);
        }

        function updateRelayTelemetry(relayId, active, remaining) {
            if (active === undefined) return;
            updateIndicator("r" + relayId + "-ind", active);
            document.getElementById("r" + relayId + "-state").innerText = active ? "On" : "Off";
            document.getElementById("r" + relayId + "-remaining").innerText = remaining || 0;
        }

        function updateProgramTelemetry(data) {
            if (data.programRunning === undefined) return;
            const badge = document.getElementById("program-run-badge");
            const status = document.getElementById("program-status");
            const line = document.getElementById("program-line");

            badge.className = "badge";
            if (data.programRunning) {
                badge.innerText = "Running";
                badge.classList.add("badge-moving");
            } else if (String(data.programStatus || "").startsWith("Error")) {
                badge.innerText = "Error";
                badge.classList.add("badge-error");
            } else if (data.programStatus === "Completed") {
                badge.innerText = "Done";
                badge.classList.add("badge-idle");
            } else {
                badge.innerText = "Idle";
                badge.classList.add("badge-disabled");
            }

            status.innerText = data.programStatus || "Stopped";
            line.innerText = data.programLine || "-";
        }

        function populateConfigFields(data) {
            if (document.activeElement && document.activeElement.tagName === "INPUT") return;
            if (data.m1Cfg) fillMotorConfig(1, data.m1Cfg);
            if (data.m2Cfg) fillMotorConfig(2, data.m2Cfg);
            if (data.sysCfg) {
                document.getElementById("sys-cfg-timeout-active").value = data.sysCfg.timeoutActive ? "1" : "0";
                document.getElementById("sys-cfg-timeout-duration").value = data.sysCfg.timeoutDuration;
            }
        }

        function fillMotorConfig(id, cfg) {
            document.getElementById("m" + id + "-cfg-speed").value = cfg.maxSpeed;
            document.getElementById("m" + id + "-cfg-accel").value = cfg.acceleration;
            document.getElementById("m" + id + "-cfg-min").value = cfg.minPosition;
            document.getElementById("m" + id + "-cfg-max").value = cfg.maxPosition;
            document.getElementById("m" + id + "-cfg-angle").value = cfg.stepAngle;
            document.getElementById("m" + id + "-cfg-usteps").value = cfg.microsteps;
            document.getElementById("m" + id + "-cfg-drive").value = cfg.driveType;
            document.getElementById("m" + id + "-cfg-pitch").value = cfg.leadScrewPitch;
            document.getElementById("m" + id + "-cfg-beltpitch").value = cfg.beltPitch;
            document.getElementById("m" + id + "-cfg-teeth").value = cfg.pulleyTeeth;
            document.getElementById("m" + id + "-cfg-unit").value = cfg.unitType;
            toggleDriveInputs(id);
        }

        function updateWsStatus(connected, text) {
            document.getElementById("ws-status-dot").classList.toggle("connected", connected);
            document.getElementById("ws-status-text").innerText = text;
        }

        function updateIndicator(id, active) {
            const el = document.getElementById(id);
            if (el) el.classList.toggle("active", !!active);
        }

        function getUnitName(val) {
            switch (val) {
                case 1: return "mm";
                case 2: return "cm";
                case 3: return "deg";
                default: return "pulse";
            }
        }

        function setMotorStatusBadge(motorId, status) {
            const badge = document.getElementById("m" + motorId + "-status-badge");
            badge.className = "badge";
            switch (status) {
                case 0:
                    badge.innerText = "Disabled";
                    badge.classList.add("badge-disabled");
                    break;
                case 1:
                    badge.innerText = "Ready";
                    badge.classList.add("badge-idle");
                    break;
                case 2:
                    badge.innerText = "Moving";
                    badge.classList.add("badge-moving");
                    break;
                case 3:
                    badge.innerText = "Homing";
                    badge.classList.add("badge-homing");
                    break;
                default:
                    badge.innerText = "Error";
                    badge.classList.add("badge-error");
                    break;
            }
        }

        function formatUptime(seconds) {
            const hrs = Math.floor(seconds / 3600);
            const mins = Math.floor((seconds % 3600) / 60);
            const secs = seconds % 60;
            return [hrs, mins, secs].map(v => String(v).padStart(2, "0")).join(":");
        }

        function logConsole(message, type = "info") {
            const consoleEl = document.getElementById("sys-console");
            const now = new Date();
            const time = "[" + String(now.getHours()).padStart(2, "0") + ":" + String(now.getMinutes()).padStart(2, "0") + ":" + String(now.getSeconds()).padStart(2, "0") + "]";
            const cls = type === "error" ? "console-err" : (type === "warn" ? "console-warn" : "");
            consoleEl.innerHTML += '<div class="console-entry ' + cls + '"><span class="console-time">' + time + '</span><span>' + escapeHtml(message) + '</span></div>';
            consoleEl.scrollTop = consoleEl.scrollHeight;
        }

        function escapeHtml(value) {
            return String(value).replace(/[&<>"']/g, ch => ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#039;" }[ch]));
        }

        function switchTab(evt, tabId) {
            activeTab = tabId;
            document.querySelectorAll(".tab").forEach(t => t.classList.remove("active"));
            document.querySelectorAll(".tab-content").forEach(c => c.classList.remove("active"));
            evt.currentTarget.classList.add("active");
            document.getElementById("tab-" + tabId).classList.add("active");
            if (tabId === "settings") sendWsMsg({ cmd: "req_config" });
        }

        function toggleDriveInputs(motorId) {
            const drive = document.getElementById("m" + motorId + "-cfg-drive").value;
            document.getElementById("m" + motorId + "-drive-screw").style.display = drive === "0" ? "flex" : "none";
            document.getElementById("m" + motorId + "-drive-belt").style.display = drive === "0" ? "none" : "grid";
        }

        function sendWsMsg(obj) {
            if (ws && ws.readyState === WebSocket.OPEN) {
                ws.send(JSON.stringify(obj));
            } else {
                logConsole("Action failed: WebSocket is not connected.", "error");
            }
        }

        function moveMotor(motorId) {
            const val = parseFloat(document.getElementById("m" + motorId + "-goto-input").value);
            if (isNaN(val)) {
                logConsole("Invalid target position for Motor " + motorId + ".", "error");
                return;
            }
            logConsole("Motor " + motorId + " moving to " + val + ".");
            sendWsMsg({ cmd: "move", motor: motorId, val: val });
        }

        function jogMotor(motorId, amount) {
            logConsole("Motor " + motorId + " jog: " + amount + ".");
            sendWsMsg({ cmd: "jog", motor: motorId, val: amount });
        }

        function stopMotor(motorId) {
            logConsole("Stopping Motor " + motorId + ".");
            sendWsMsg({ cmd: "stop", motor: motorId });
        }

        function homeMotor(motorId) {
            logConsole("Starting homing cycle for Motor " + motorId + ".");
            sendWsMsg({ cmd: "home", motor: motorId });
        }

        function resetMotorPos(motorId) {
            logConsole("Resetting Motor " + motorId + " position to zero.");
            sendWsMsg({ cmd: "reset", motor: motorId });
        }

        function enableMotor(motorId, state) {
            logConsole((state ? "Enabling" : "Disabling") + " Motor " + motorId + " driver.");
            sendWsMsg({ cmd: "enable", motor: motorId, val: state ? 1 : 0 });
        }

        function sendAllStop() {
            logConsole("Emergency stop requested. Motors and general outputs are being stopped.", "error");
            sendWsMsg({ cmd: "allstop" });
        }

        function runProgram() {
            const editor = document.getElementById("program-editor");
            const program = editor.value.trim();
            if (!program.length) {
                logConsole("Program is empty.", "error");
                return;
            }
            localStorage.setItem("motionProgramText", editor.value);
            logConsole("Program sent to ESP32.");
            sendWsMsg({ cmd: "program_start", program: editor.value });
        }

        function stopProgram() {
            logConsole("Program stop requested.", "warn");
            sendWsMsg({ cmd: "program_stop" });
        }

        function triggerRelay(relayId) {
            const input = document.getElementById("r" + relayId + "-duration");
            let duration = parseInt(input.value, 10);
            if (isNaN(duration) || duration < 1) duration = 1;
            if (duration > 86400) duration = 86400;
            input.value = duration;
            logConsole("Gen.Out" + relayId + " triggered for " + duration + " seconds.");
            sendWsMsg({ cmd: "relay_trigger", relay: relayId, duration: duration });
        }

        function turnRelayOff(relayId) {
            logConsole("Gen.Out" + relayId + " manual off requested.");
            sendWsMsg({ cmd: "relay_off", relay: relayId });
        }

        function rebootESP() {
            if (confirm("Restart the ESP32 now?")) {
                logConsole("Restart command sent.", "warn");
                sendWsMsg({ cmd: "reboot" });
            }
        }

        function saveMotorSettings() {
            const cfg = {
                cmd: "save_config",
                m1: readMotorConfig(1),
                m2: readMotorConfig(2),
                sys: {
                    timeoutActive: document.getElementById("sys-cfg-timeout-active").value === "1",
                    timeoutDuration: parseInt(document.getElementById("sys-cfg-timeout-duration").value, 10)
                }
            };
            logConsole("Sending motor settings.");
            sendWsMsg(cfg);
        }

        function readMotorConfig(id) {
            return {
                maxSpeed: parseFloat(document.getElementById("m" + id + "-cfg-speed").value),
                acceleration: parseFloat(document.getElementById("m" + id + "-cfg-accel").value),
                minPosition: parseInt(document.getElementById("m" + id + "-cfg-min").value, 10),
                maxPosition: parseInt(document.getElementById("m" + id + "-cfg-max").value, 10),
                stepAngle: parseFloat(document.getElementById("m" + id + "-cfg-angle").value),
                microsteps: parseInt(document.getElementById("m" + id + "-cfg-usteps").value, 10),
                driveType: parseInt(document.getElementById("m" + id + "-cfg-drive").value, 10),
                leadScrewPitch: parseFloat(document.getElementById("m" + id + "-cfg-pitch").value),
                beltPitch: parseFloat(document.getElementById("m" + id + "-cfg-beltpitch").value),
                pulleyTeeth: parseInt(document.getElementById("m" + id + "-cfg-teeth").value, 10),
                unitType: parseInt(document.getElementById("m" + id + "-cfg-unit").value, 10)
            };
        }

        function saveWifiSettings() {
            const ssid = document.getElementById("wifi-ssid").value;
            const pass = document.getElementById("wifi-pass").value;
            if (!ssid.length) {
                alert("Wi-Fi SSID cannot be empty.");
                return;
            }
            logConsole("Saving Wi-Fi settings. Restart the ESP32 to apply them.");
            sendWsMsg({ cmd: "save_wifi", ssid: ssid, pass: pass });
        }

        window.addEventListener("load", function() {
            const savedProgram = localStorage.getItem("motionProgramText");
            if (savedProgram) {
                const migratedProgram = savedProgram
                    .replace(/^R1\s+/gmi, "Gen1 ")
                    .replace(/^R2\s+/gmi, "Gen2 ");
                document.getElementById("program-editor").value = migratedProgram;
                if (migratedProgram !== savedProgram) localStorage.setItem("motionProgramText", migratedProgram);
            }
            initWebSocket();
        });
    </script>
</body>
</html>
)rawhtml";

#endif // INDEX_HTML_H
