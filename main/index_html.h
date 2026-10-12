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
            --bg: #f1f1f3;
            --panel: #ffffff;
            --panel-2: #fff7ed;
            --border: #fd8208;
            --soft-border: rgba(253, 130, 8, 0.28);
            --text: #1f2933;
            --muted: #6b7280;
            --primary: #fd8208;
            --primary-dark: #b85600;
            --primary-soft: rgba(253, 130, 8, 0.14);
            --success: #138a5b;
            --danger: #c42147;
            --warning: #b76b00;
            --accent: #ffb15c;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
            font-family: Inter, Segoe UI, Arial, sans-serif;
        }

        body {
            min-height: 100vh;
            background: var(--bg);
            color: var(--text);
            padding: 20px;
        }

        body::before {
            content: "";
            position: fixed;
            inset: 0;
            z-index: -1;
            pointer-events: none;
            background:
                linear-gradient(135deg, rgba(253, 130, 8, 0.10) 0 10%, transparent 10% 76%, rgba(253, 130, 8, 0.07) 76% 100%),
                var(--bg);
        }

        header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 16px;
            padding-bottom: 18px;
            border-bottom: 1px solid var(--soft-border);
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
            background: #d1d5db;
        }

        .status-dot.connected,
        .indicator.active {
            background: var(--success);
            box-shadow: 0 0 0 3px rgba(19, 138, 91, 0.16);
        }

        .tabs {
            display: flex;
            gap: 10px;
            padding-bottom: 14px;
            margin-bottom: 18px;
            border-bottom: 1px solid var(--soft-border);
            overflow-x: auto;
        }

        .tab {
            padding: 10px 16px;
            border-radius: 8px;
            border: 1px solid var(--border);
            color: var(--muted);
            background: rgba(255, 255, 255, 0.86);
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
            grid-template-columns: repeat(auto-fit, minmax(270px, max-content));
            justify-content: start;
            align-items: stretch;
            gap: 10px;
        }

        .settings-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(320px, 420px));
            justify-content: start;
            gap: 18px;
        }

        .settings-card {
            width: min(100%, 896px);
        }

        .settings-utility-card,
        .settings-actions {
            width: min(100%, 858px);
        }

        .settings-actions {
            display: flex;
            justify-content: flex-end;
        }

        .card {
            background: var(--panel);
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 18px;
            display: flex;
            flex-direction: column;
            gap: 14px;
            box-shadow: 0 10px 22px rgba(253, 130, 8, 0.10);
        }

        .wide-card {
            grid-column: 1 / -1;
        }

        .dashboard-grid > .card {
            width: min(100%, 295px);
        }

        .dashboard-grid > .relay-card {
            width: min(100%, 295px);
        }

        .dashboard-grid > .dashboard-manual-relay {
            grid-column: auto;
            width: min(100%, 295px);
        }

        .dashboard-grid > .dashboard-system-card {
            width: min(100%, 740px);
        }

        .card-title {
            display: flex;
            align-items: center;
            justify-content: space-between;
            gap: 12px;
            padding-bottom: 10px;
            border-bottom: 1px solid var(--soft-border);
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
            border: 1px solid var(--soft-border);
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

        .badge-disabled { background: #f3f4f6; color: #6b7280; border: 1px solid #d1d5db; }
        .badge-idle { background: var(--primary-soft); color: var(--primary-dark); border: 1px solid var(--soft-border); }
        .badge-moving { background: rgba(19, 138, 91, 0.12); color: var(--success); border: 1px solid rgba(19, 138, 91, 0.24); }
        .badge-homing { background: rgba(183, 107, 0, 0.12); color: var(--warning); border: 1px solid rgba(183, 107, 0, 0.24); }
        .badge-error { background: rgba(196, 33, 71, 0.12); color: var(--danger); border: 1px solid rgba(196, 33, 71, 0.24); }

        .progress-container {
            height: 10px;
            overflow: hidden;
            border-radius: 999px;
            background: #ffe8cf;
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
            background: #ffffff;
            color: var(--text);
            border: 1px solid var(--border);
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

        .program-editor-shell {
            display: grid;
            grid-template-columns: 52px minmax(0, 1fr);
            align-items: stretch;
            min-height: 300px;
        }

        .program-line-numbers {
            min-height: 300px;
            overflow: hidden;
            padding: 10px 8px 10px 4px;
            background: #fff7ed;
            border: 1px solid var(--border);
            border-right: 0;
            border-radius: 8px 0 0 8px;
            color: var(--muted);
            font-family: Consolas, monospace;
            font-size: 0.93rem;
            line-height: 1.45;
            text-align: right;
            user-select: none;
        }

        .program-line-number {
            height: 1.45em;
            padding-right: 4px;
        }

        .program-line-number.active {
            color: #ffffff;
            background: var(--primary);
            border-radius: 4px;
            font-weight: 700;
        }

        #program-editor {
            min-width: 0;
            margin: 0;
            border-radius: 0 8px 8px 0;
            white-space: pre;
            overflow: auto;
        }

        .program-editor-shell:focus-within .program-line-numbers {
            border-color: var(--primary);
        }

        input:focus,
        select:focus,
        textarea:focus {
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(253, 130, 8, 0.20);
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

        .jog-step-selector {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 7px;
        }

        .jog-step-selector .btn {
            padding: 9px 8px;
            background: #ffffff;
            color: var(--text);
            border-color: var(--border);
        }

        .jog-step-selector .btn.active {
            color: #ffffff;
            background: var(--primary);
            border-color: var(--primary);
            box-shadow: 0 0 0 3px rgba(253, 130, 8, 0.16);
        }

        .btn:disabled {
            cursor: not-allowed;
            opacity: 0.42;
            box-shadow: none;
        }

        .relay-grid {
            display: grid;
            grid-template-columns: 1fr;
            gap: 14px;
        }

        .relay-card .stat-grid {
            grid-template-columns: 1fr;
        }

        .dashboard-relay-grid {
            display: grid;
            grid-template-columns: 1fr;
            gap: 12px;
        }

        .relay-test-actions {
            display: grid;
            grid-template-columns: repeat(2, minmax(0, 1fr));
            gap: 9px;
            margin-top: 10px;
        }

        .program-grid {
            display: grid;
            grid-template-columns: repeat(2, minmax(0, 560px));
            justify-content: start;
            gap: 18px;
            align-items: stretch;
        }

        .program-status {
            display: grid;
            grid-template-columns: repeat(3, minmax(0, 1fr));
            gap: 10px;
        }

        .program-main-card {
            width: 100%;
        }

        .program-main-card .program-status {
            grid-template-columns: repeat(2, minmax(0, 1fr));
        }

        .program-main-card .program-editor-shell,
        .program-main-card textarea,
        .program-main-card .program-line-numbers {
            min-height: 260px;
        }

        .program-selector {
            display: grid;
            grid-template-columns: repeat(2, minmax(0, 1fr));
            gap: 8px;
            padding: 5px;
            border: 1px solid var(--border);
            border-radius: 8px;
            background: #fff4e8;
        }

        .program-choice {
            min-height: 38px;
            border: 1px solid transparent;
            border-radius: 6px;
            background: transparent;
            color: var(--primary-dark);
            cursor: pointer;
            font-weight: 800;
        }

        .program-choice.active {
            color: #ffffff;
            background: var(--primary);
        }

        .program-choice:disabled {
            cursor: not-allowed;
            opacity: 0.65;
        }

        .program-support-card {
            grid-column: span 1;
        }

        .command-list {
            display: grid;
            gap: 8px;
            color: var(--muted);
            font-size: 0.84rem;
            line-height: 1.45;
        }

        .command-list code {
            color: var(--primary-dark);
            font-family: Consolas, monospace;
        }

        .btn,
        .emergency-btn {
            min-height: 40px;
            border: 1px solid var(--border);
            border-radius: 8px;
            color: var(--primary-dark);
            background: #ffffff;
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

        .btn-primary { background: var(--primary); color: #ffffff; border-color: transparent; }
        .btn-success { background: #fd8208; color: #ffffff; border-color: transparent; }
        .btn-warning { background: #ffd7ad; color: var(--primary-dark); border-color: var(--border); }
        .btn-danger,
        .emergency-btn { background: var(--primary-dark); color: #ffffff; border-color: transparent; }

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
            background: #ffffff;
            border: 1px solid var(--border);
            border-radius: 8px;
            padding: 12px;
            font-family: Consolas, monospace;
            font-size: 0.82rem;
            line-height: 1.45;
            color: var(--primary-dark);
        }

        .console-entry {
            display: flex;
            gap: 8px;
        }

        .console-time {
            color: var(--primary);
            flex: 0 0 auto;
        }

        .console-warn { color: var(--warning); }
        .console-err { color: var(--danger); }

        footer {
            padding: 24px 0 4px;
            text-align: center;
            color: var(--muted);
            font-size: 0.78rem;
        }

        @media (max-width: 1080px) {
            .dashboard-grid {
                grid-template-columns: repeat(auto-fit, minmax(270px, max-content));
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
            .program-status,
            .dashboard-relay-grid,
            .relay-test-actions {
                grid-template-columns: 1fr;
            }
            .dashboard-grid {
                grid-template-columns: 1fr;
            }
            .dashboard-grid > .card,
            .dashboard-grid > .relay-card,
            .dashboard-grid > .dashboard-manual-relay,
            .dashboard-grid > .dashboard-system-card {
                width: 100%;
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

                <div class="input-group">
                    <label>Manual Move Step</label>
                    <div class="jog-step-selector" role="group" aria-label="X axis manual move step">
                        <button type="button" class="btn" data-jog-motor="1" data-jog-step="0.1" aria-pressed="false" onclick="setJogStep(1, 0.1)">0.1</button>
                        <button type="button" class="btn active" data-jog-motor="1" data-jog-step="1" aria-pressed="true" onclick="setJogStep(1, 1)">1</button>
                        <button type="button" class="btn" data-jog-motor="1" data-jog-step="10" aria-pressed="false" onclick="setJogStep(1, 10)">10</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button id="m1-jog-negative" class="btn btn-warning" onclick="jogMotorByStep(1, -1)">-X</button>
                    <button class="btn btn-danger" onclick="stopMotor(1)">Stop</button>
                    <button class="btn btn-success" onclick="jogMotorByStep(1, 1)">+X</button>
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

                <div class="input-group">
                    <label>Manual Move Step</label>
                    <div class="jog-step-selector" role="group" aria-label="Y axis manual move step">
                        <button type="button" class="btn" data-jog-motor="2" data-jog-step="0.1" aria-pressed="false" onclick="setJogStep(2, 0.1)">0.1</button>
                        <button type="button" class="btn active" data-jog-motor="2" data-jog-step="1" aria-pressed="true" onclick="setJogStep(2, 1)">1</button>
                        <button type="button" class="btn" data-jog-motor="2" data-jog-step="10" aria-pressed="false" onclick="setJogStep(2, 10)">10</button>
                    </div>
                </div>

                <div class="jog-grid">
                    <button id="m2-jog-negative" class="btn btn-warning" onclick="jogMotorByStep(2, -1)">-Y</button>
                    <button class="btn btn-danger" onclick="stopMotor(2)">Stop</button>
                    <button class="btn btn-success" onclick="jogMotorByStep(2, 1)">+Y</button>
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
                        <div class="subtitle">Gen.Out1 uses GPIO23. Gen.Out2 uses GPIO5. Timed output duration is entered and counted in milliseconds.</div>
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
                                <label for="r1-duration">On Time (ms)</label>
                                <input type="number" id="r1-duration" min="1" max="86400000" value="5000">
                            </div>
                            <div class="stat-box">
                                <span class="stat-label">Remaining</span>
                                <span class="stat-value"><span id="r1-remaining">0</span><span class="stat-unit">ms</span></span>
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
                                <label for="r2-duration">On Time (ms)</label>
                                <input type="number" id="r2-duration" min="1" max="86400000" value="5000">
                            </div>
                            <div class="stat-box">
                                <span class="stat-label">Remaining</span>
                                <span class="stat-value"><span id="r2-remaining">0</span><span class="stat-unit">ms</span></span>
                            </div>
                        </div>
                        <div class="button-row" style="margin-top:10px">
                            <button class="btn btn-success" onclick="triggerRelay(2)">Trigger Gen.Out2</button>
                            <button class="btn btn-danger" onclick="turnRelayOff(2)">Turn Off</button>
                        </div>
                    </div>
                </div>
            </article>

            <article class="card wide-card dashboard-manual-relay">
                <div class="card-title">
                    <div>
                        <h2>Program Relay Manual Test</h2>
                        <div class="subtitle">Direct ON/OFF controls for R1, R2, and R3. These are the same outputs used by Program commands.</div>
                    </div>
                </div>
                <div class="dashboard-relay-grid">
                    <div class="stat-box">
                        <div class="card-title" style="border:0;padding:0;margin-bottom:8px">
                            <div>
                                <h3>R1</h3>
                                <span class="stat-label">GPIO<span id="dashboard-relay-r1-pin">2</span></span>
                            </div>
                            <span class="status-badge"><span id="dashboard-relay-r1-ind" class="indicator"></span><span id="dashboard-relay-r1-state">Off</span></span>
                        </div>
                        <div class="relay-test-actions">
                            <button class="btn btn-success" onclick="setProgramRelay(1, true)">Turn On</button>
                            <button class="btn btn-danger" onclick="setProgramRelay(1, false)">Turn Off</button>
                        </div>
                    </div>
                    <div class="stat-box">
                        <div class="card-title" style="border:0;padding:0;margin-bottom:8px">
                            <div>
                                <h3>R2</h3>
                                <span class="stat-label">GPIO<span id="dashboard-relay-r2-pin">12</span></span>
                            </div>
                            <span class="status-badge"><span id="dashboard-relay-r2-ind" class="indicator"></span><span id="dashboard-relay-r2-state">Off</span></span>
                        </div>
                        <div class="relay-test-actions">
                            <button class="btn btn-success" onclick="setProgramRelay(2, true)">Turn On</button>
                            <button class="btn btn-danger" onclick="setProgramRelay(2, false)">Turn Off</button>
                        </div>
                    </div>
                    <div class="stat-box">
                        <div class="card-title" style="border:0;padding:0;margin-bottom:8px">
                            <div>
                                <h3>R3</h3>
                                <span class="stat-label">GPIO<span id="dashboard-relay-r3-pin">15</span></span>
                            </div>
                            <span class="status-badge"><span id="dashboard-relay-r3-ind" class="indicator"></span><span id="dashboard-relay-r3-state">Off</span></span>
                        </div>
                        <div class="relay-test-actions">
                            <button class="btn btn-success" onclick="setProgramRelay(3, true)">Turn On</button>
                            <button class="btn btn-danger" onclick="setProgramRelay(3, false)">Turn Off</button>
                        </div>
                    </div>
                </div>
            </article>

            <article class="card wide-card dashboard-system-card">
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
        <article class="card settings-card">
            <div class="card-title"><h2>Motor Mechanical And Limit Configuration</h2></div>
            <div class="settings-grid">
                <div style="display:flex;flex-direction:column;gap:12px">
                    <h3>Motor 1 - X Axis</h3>
                    <div class="stat-grid">
                        <div class="input-group"><label>Speed Limit (steps/s)</label><input type="number" id="m1-cfg-speed" value="2000"></div>
                        <div class="input-group"><label>Acceleration (steps/s2)</label><input type="number" id="m1-cfg-accel" value="1000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Software Minimum Limit (mm)</label><input type="number" id="m1-cfg-min" step="0.01" value="0" oninput="updateLimitStepInfo(1)"><span class="subtitle" id="m1-cfg-min-steps">Calculated: 0 steps</span></div>
                        <div class="input-group"><label>Software Maximum Limit (mm)</label><input type="number" id="m1-cfg-max" step="0.01" value="100" oninput="updateLimitStepInfo(1)"><span class="subtitle" id="m1-cfg-max-steps">Calculated: 0 steps</span></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Step Angle (degrees)</label><input type="number" id="m1-cfg-angle" step="0.1" value="1.8" oninput="updateLimitStepInfo(1)"></div>
                        <div class="input-group">
                            <label>Microstep (1/x)</label>
                            <select id="m1-cfg-usteps" onchange="updateLimitStepInfo(1)">
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
                        <select id="m1-cfg-drive" onchange="toggleDriveInputs(1);updateLimitStepInfo(1)">
                            <option value="0" selected>Lead Screw</option>
                            <option value="1">Belt And Pulley</option>
                        </select>
                    </div>
                    <div id="m1-drive-screw" class="input-group"><label>Lead Screw Pitch (mm/rev)</label><input type="number" id="m1-cfg-pitch" step="0.01" value="8.00" oninput="updateLimitStepInfo(1)"></div>
                    <div id="m1-drive-belt" class="stat-grid" style="display:none">
                        <div class="input-group"><label>Belt Pitch (mm)</label><input type="number" id="m1-cfg-beltpitch" step="0.01" value="2.00" oninput="updateLimitStepInfo(1)"></div>
                        <div class="input-group"><label>Pulley Teeth</label><input type="number" id="m1-cfg-teeth" value="20" oninput="updateLimitStepInfo(1)"></div>
                    </div>
                    <div class="input-group">
                        <label>Interface Unit</label>
                        <select id="m1-cfg-unit" onchange="updateHomeUnitLabels(1)">
                            <option value="0">Pulse</option>
                            <option value="1" selected>Millimeter (mm)</option>
                            <option value="2">Centimeter (cm)</option>
                            <option value="3">Degree</option>
                        </select>
                    </div>
                    <div class="stat-box">
                        <h3>Home Settings</h3>
                        <div class="subtitle">Independent homing profile. Speeds and distances use the selected interface unit and remain physically consistent when microstep changes.</div>
                        <div class="stat-grid" style="margin-top:10px">
                            <div class="input-group"><label>Fast Search Speed (<span id="m1-home-unit-speed">mm/s</span>)</label><input type="number" id="m1-cfg-home-fast" min="0.01" step="0.01" value="4.00"></div>
                            <div class="input-group"><label>Slow Approach Speed (<span id="m1-home-unit-slow">mm/s</span>)</label><input type="number" id="m1-cfg-home-slow" min="0.01" step="0.01" value="1.50"></div>
                            <div class="input-group"><label>Home Acceleration (<span id="m1-home-unit-accel">mm/s2</span>)</label><input type="number" id="m1-cfg-home-accel" min="0.01" step="0.01" value="3.00"></div>
                            <div class="input-group"><label>Extra Clearance (<span id="m1-home-unit-backoff">mm</span>)</label><input type="number" id="m1-cfg-home-backoff" min="0.01" step="0.01" value="1.00"></div>
                            <div class="input-group"><label>Maximum Switch Release Travel (<span id="m1-home-unit-release">mm</span>)</label><input type="number" id="m1-cfg-home-max-release" min="0.01" step="0.01" value="5.00"></div>
                            <div class="input-group"><label>Direction Settle (ms)</label><input type="number" id="m1-cfg-home-settle" min="10" max="2000" value="60"></div>
                            <div class="input-group"><label>Switch Debounce (ms)</label><input type="number" id="m1-cfg-home-debounce" min="5" max="500" value="30"></div>
                            <div class="input-group"><label>Search Timeout (s)</label><input type="number" id="m1-cfg-home-timeout" min="1" max="120" step="0.1" value="30"></div>
                        </div>
                    </div>
                </div>

                <div style="display:flex;flex-direction:column;gap:12px">
                    <h3>Motor 2 - Y Axis</h3>
                    <div class="stat-grid">
                        <div class="input-group"><label>Speed Limit (steps/s)</label><input type="number" id="m2-cfg-speed" value="2000"></div>
                        <div class="input-group"><label>Acceleration (steps/s2)</label><input type="number" id="m2-cfg-accel" value="1000"></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Software Minimum Limit (mm)</label><input type="number" id="m2-cfg-min" step="0.01" value="0" oninput="updateLimitStepInfo(2)"><span class="subtitle" id="m2-cfg-min-steps">Calculated: 0 steps</span></div>
                        <div class="input-group"><label>Software Maximum Limit (mm)</label><input type="number" id="m2-cfg-max" step="0.01" value="100" oninput="updateLimitStepInfo(2)"><span class="subtitle" id="m2-cfg-max-steps">Calculated: 0 steps</span></div>
                    </div>
                    <div class="stat-grid">
                        <div class="input-group"><label>Step Angle (degrees)</label><input type="number" id="m2-cfg-angle" step="0.1" value="1.8" oninput="updateLimitStepInfo(2)"></div>
                        <div class="input-group">
                            <label>Microstep (1/x)</label>
                            <select id="m2-cfg-usteps" onchange="updateLimitStepInfo(2)">
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
                        <select id="m2-cfg-drive" onchange="toggleDriveInputs(2);updateLimitStepInfo(2)">
                            <option value="0" selected>Lead Screw</option>
                            <option value="1">Belt And Pulley</option>
                        </select>
                    </div>
                    <div id="m2-drive-screw" class="input-group"><label>Lead Screw Pitch (mm/rev)</label><input type="number" id="m2-cfg-pitch" step="0.01" value="8.00" oninput="updateLimitStepInfo(2)"></div>
                    <div id="m2-drive-belt" class="stat-grid" style="display:none">
                        <div class="input-group"><label>Belt Pitch (mm)</label><input type="number" id="m2-cfg-beltpitch" step="0.01" value="2.00" oninput="updateLimitStepInfo(2)"></div>
                        <div class="input-group"><label>Pulley Teeth</label><input type="number" id="m2-cfg-teeth" value="20" oninput="updateLimitStepInfo(2)"></div>
                    </div>
                    <div class="input-group">
                        <label>Interface Unit</label>
                        <select id="m2-cfg-unit" onchange="updateHomeUnitLabels(2)">
                            <option value="0">Pulse</option>
                            <option value="1" selected>Millimeter (mm)</option>
                            <option value="2">Centimeter (cm)</option>
                            <option value="3">Degree</option>
                        </select>
                    </div>
                    <div class="stat-box">
                        <h3>Home Settings</h3>
                        <div class="subtitle">Independent homing profile. Speeds and distances use the selected interface unit and remain physically consistent when microstep changes.</div>
                        <div class="stat-grid" style="margin-top:10px">
                            <div class="input-group"><label>Fast Search Speed (<span id="m2-home-unit-speed">mm/s</span>)</label><input type="number" id="m2-cfg-home-fast" min="0.01" step="0.01" value="4.00"></div>
                            <div class="input-group"><label>Slow Approach Speed (<span id="m2-home-unit-slow">mm/s</span>)</label><input type="number" id="m2-cfg-home-slow" min="0.01" step="0.01" value="1.50"></div>
                            <div class="input-group"><label>Home Acceleration (<span id="m2-home-unit-accel">mm/s2</span>)</label><input type="number" id="m2-cfg-home-accel" min="0.01" step="0.01" value="3.00"></div>
                            <div class="input-group"><label>Extra Clearance (<span id="m2-home-unit-backoff">mm</span>)</label><input type="number" id="m2-cfg-home-backoff" min="0.01" step="0.01" value="1.00"></div>
                            <div class="input-group"><label>Maximum Switch Release Travel (<span id="m2-home-unit-release">mm</span>)</label><input type="number" id="m2-cfg-home-max-release" min="0.01" step="0.01" value="5.00"></div>
                            <div class="input-group"><label>Direction Settle (ms)</label><input type="number" id="m2-cfg-home-settle" min="10" max="2000" value="60"></div>
                            <div class="input-group"><label>Switch Debounce (ms)</label><input type="number" id="m2-cfg-home-debounce" min="5" max="500" value="30"></div>
                            <div class="input-group"><label>Search Timeout (s)</label><input type="number" id="m2-cfg-home-timeout" min="1" max="120" step="0.1" value="30"></div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="stat-box settings-utility-card">
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
            <div class="settings-actions">
                <button class="btn btn-primary" onclick="saveMotorSettings()">Save And Apply Settings</button>
            </div>
        </article>
    </section>

    <section id="tab-program" class="tab-content">
        <div class="program-grid">
            <article class="card program-main-card">
                <div class="card-title">
                    <div>
                        <h2>Sequential Program</h2>
                        <div class="subtitle">Program 1 and Program 2 have separate ESP32 memories. Run Program and GPIO34 start the selected program.</div>
                    </div>
                    <span id="program-run-badge" class="badge badge-idle">Idle</span>
                </div>

                <div class="input-group">
                    <label>Select Program Memory</label>
                    <div class="program-selector" role="group" aria-label="Sequential Program memory">
                        <button id="program-choice-1" class="program-choice active" type="button" onclick="selectSequentialProgram(1)">Program 1</button>
                        <button id="program-choice-2" class="program-choice" type="button" onclick="selectSequentialProgram(2)">Program 2</button>
                    </div>
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
                    <div class="stat-box">
                        <span class="stat-label" id="program-saved-label">Program 1 Saved</span>
                        <span class="stat-value" id="program-saved-length" style="font-size:0.9rem">0 chars</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">GPIO34 Button</span>
                        <span class="stat-value" style="font-size:0.9rem"><span id="program-button-ind" class="indicator"></span> <span id="program-button-state">Released</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Button Starts</span>
                        <span class="stat-value" id="program-button-count" style="font-size:0.9rem">0</span>
                    </div>
                </div>

                <div class="input-group">
                    <label for="program-editor" id="program-editor-label">Program 1 Text</label>
                    <div class="program-editor-shell">
                        <div id="program-line-numbers" class="program-line-numbers" aria-hidden="true"></div>
                        <textarea id="program-editor" spellcheck="false" oninput="programEditorChanged()" onscroll="syncProgramEditorScroll()"># X and Y values are relative movement in the active motor unit
# VX and VY values are speed in steps/s
# X HOME and Y HOME start homing cycles
VX 5000
VY 5000
X 50
Y 10
D 50
Gen1 ON
R1 ON
D 100
X -50
Gen2 OFF
R2 ON
D 200
R1 OFF
R2 OFF
R3 ON
D 100
R3 OFF
Gen1 OFF</textarea>
                    </div>
                </div>

                <div class="button-row">
                    <button class="btn btn-success" onclick="runProgram()">Run Program</button>
                    <button class="btn btn-danger" onclick="stopProgram()">Stop Program</button>
                </div>
            </article>

            <article class="card program-main-card">
                <div class="card-title">
                    <div>
                        <h2>Reset Program</h2>
                        <div class="subtitle">Separate saved program text. Reset Program runs this text from the button below or the GPIO35 reset input.</div>
                    </div>
                </div>

                <div class="program-status">
                    <div class="stat-box">
                        <span class="stat-label">Saved Text</span>
                        <span class="stat-value" id="reset-program-saved-length" style="font-size:0.9rem">0 chars</span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">GPIO35 Button</span>
                        <span class="stat-value" style="font-size:0.9rem"><span id="reset-program-button-ind" class="indicator"></span> <span id="reset-program-button-state">Released</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">Button Starts</span>
                        <span class="stat-value" id="reset-program-button-count" style="font-size:0.9rem">0</span>
                    </div>
                </div>

                <div class="input-group">
                    <label for="reset-program-editor">Reset Program Text</label>
                    <div class="program-editor-shell">
                        <div id="reset-program-line-numbers" class="program-line-numbers" aria-hidden="true"></div>
                        <textarea id="reset-program-editor" spellcheck="false" oninput="resetProgramEditorChanged()" onscroll="syncProgramEditorScroll('reset')"># Reset Program example
# Commands are the same as Run Program
X HOME
Y HOME
D 100
Gen1 OFF
Gen2 OFF
R1 OFF
R2 OFF
R3 OFF</textarea>
                    </div>
                </div>

                <div class="button-row">
                    <button class="btn btn-success" onclick="runResetProgram()">Reset Program</button>
                    <button class="btn btn-danger" onclick="stopProgram()">Stop Program</button>
                </div>
            </article>

            <article class="card program-support-card">
                <div class="card-title">
                    <h2>Program Help</h2>
                </div>
                <div class="command-list">
                    <div><code>X 50</code> moves the X axis, Motor 1, +50 relative units.</div>
                    <div><code>X -50</code> moves the X axis, Motor 1, -50 relative units.</div>
                    <div><code>Y 10</code> moves the Y axis, Motor 2, +10 relative units.</div>
                    <div><code>VX 5000</code> sets X axis, Motor 1, speed to 5000 steps/s.</div>
                    <div><code>VY 5000</code> sets Y axis, Motor 2, speed to 5000 steps/s.</div>
                    <div><code>X HOME</code> starts X axis, Motor 1, homing and waits until it finishes.</div>
                    <div><code>Y HOME</code> starts Y axis, Motor 2, homing and waits until it finishes.</div>
                    <div><code>D 100</code> waits 100 milliseconds before the next line.</div>
                    <div><code>Gen1 ON</code> turns Gen.Out1 on until another command turns it off.</div>
                    <div><code>Gen2 OFF</code> turns Gen.Out2 off.</div>
                    <div><code>R1 ON</code> turns the R1 relay output on.</div>
                    <div><code>R2 OFF</code> turns the R2 relay output off.</div>
                    <div><code>R3 ON</code> turns the R3 relay output on.</div>
                    <div><code>GPIO34</code> physical button starts the last saved Run Program text.</div>
                    <div><code>GPIO35</code> physical button starts the last saved Reset Program text.</div>
                    <div>If a program is already running, new Run Program or Reset Program start requests are ignored.</div>
                    <div>Use <code>#</code> or <code>//</code> for comments. Empty lines are ignored.</div>
                    <div>The movement unit is the unit selected in Motor Settings: mm, cm, degree, or pulse.</div>
                </div>
            </article>

            <article class="card program-support-card">
                <div class="card-title">
                    <h2>Program Relay Outputs</h2>
                </div>
                <div class="program-status">
                    <div class="stat-box">
                        <span class="stat-label">R1 GPIO<span id="relay-r1-pin"></span></span>
                        <span class="stat-value"><span id="relay-r1-ind" class="indicator"></span> <span id="relay-r1-state">Off</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">R2 GPIO<span id="relay-r2-pin"></span></span>
                        <span class="stat-value"><span id="relay-r2-ind" class="indicator"></span> <span id="relay-r2-state">Off</span></span>
                    </div>
                    <div class="stat-box">
                        <span class="stat-label">R3 GPIO<span id="relay-r3-pin"></span></span>
                        <span class="stat-value"><span id="relay-r3-ind" class="indicator"></span> <span id="relay-r3-state">Off</span></span>
                    </div>
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
        let programSaveTimer = null;
        let resetProgramSaveTimer = null;
        let activeProgramLineNumber = 0;
        let activeProgramEditor = "program";
        let selectedProgramSlot = 1;
        let programIsRunning = false;
        const programTexts = { 1: "", 2: "" };

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
                        if (data.savedProgramText !== undefined || data.savedProgramText2 !== undefined) {
                            const editor = document.getElementById("program-editor");
                            const savedProgram1 = String(data.savedProgramText || "");
                            const savedProgram2 = String(data.savedProgramText2 || "");
                            const localProgram1 = localStorage.getItem("motionProgramText1") || localStorage.getItem("motionProgramText") || "";
                            const localProgram2 = localStorage.getItem("motionProgramText2") || "";
                            programTexts[1] = savedProgram1 || localProgram1 || editor.value;
                            programTexts[2] = savedProgram2 || localProgram2;
                            selectedProgramSlot = Number(data.selectedProgramSlot) === 2 ? 2 : 1;
                            editor.value = programTexts[selectedProgramSlot];
                            localStorage.setItem("motionProgramText1", programTexts[1]);
                            localStorage.setItem("motionProgramText2", programTexts[2]);
                            if (!savedProgram1 && programTexts[1]) syncProgramTextToDevice(false, 1, programTexts[1]);
                            if (!savedProgram2 && programTexts[2]) syncProgramTextToDevice(false, 2, programTexts[2]);
                            updateProgramSelector();
                            updateProgramLineNumbers();
                        }
                        if (data.savedResetProgramText !== undefined) {
                            const editor = document.getElementById("reset-program-editor");
                            const localProgram = localStorage.getItem("resetProgramText");
                            const savedProgramText = String(data.savedResetProgramText || "");
                            if (savedProgramText.length) {
                                editor.value = savedProgramText;
                                localStorage.setItem("resetProgramText", savedProgramText);
                            } else if (localProgram) {
                                editor.value = localProgram;
                                syncResetProgramTextToDevice(false);
                            } else {
                                localStorage.setItem("resetProgramText", editor.value);
                                syncResetProgramTextToDevice(false);
                            }
                            updateProgramLineNumbers();
                        }
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

            updateRelayTelemetry(1, data.r1Active, data.r1RemainingMs);
            updateRelayTelemetry(2, data.r2Active, data.r2RemainingMs);
            updateProgramTelemetry(data);
            updateProgramRelayTelemetry(data);

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

            const homeSwitchActive = !!data[prefix + "Sw"];
            updateIndicator(prefix + "-sw", homeSwitchActive);
            updateIndicator(prefix + "-en", data[prefix + "En"]);

            const negativeJogButton = document.getElementById(prefix + "-jog-negative");
            negativeJogButton.disabled = homeSwitchActive;
            negativeJogButton.title = homeSwitchActive
                ? "Home switch is active. Movement toward home is blocked."
                : "Move toward home";

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
            programIsRunning = !!data.programRunning;
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
            activeProgramEditor = data.activeProgramName === "Reset Program" ? "reset" : "program";
            const currentLineMatch = String(data.programLine || "").match(/^(\d+):/);
            setActiveProgramLine(currentLineMatch ? parseInt(currentLineMatch[1], 10) : 0);
            if (data.programSavedLength !== undefined) {
                document.getElementById("program-saved-length").innerText = data.programSavedLength + " chars";
            }
            if (data.selectedProgramSlot !== undefined && !programIsRunning) {
                const deviceSlot = Number(data.selectedProgramSlot) === 2 ? 2 : 1;
                if (deviceSlot !== selectedProgramSlot) selectSequentialProgram(deviceSlot, false);
            }
            updateProgramSelector();
            if (data.resetProgramSavedLength !== undefined) {
                document.getElementById("reset-program-saved-length").innerText = data.resetProgramSavedLength + " chars";
            }
            if (data.programStartButtonPressed !== undefined) {
                updateIndicator("program-button-ind", data.programStartButtonPressed);
                document.getElementById("program-button-state").innerText = data.programStartButtonPressed ? "Pressed" : "Released";
            }
            if (data.programStartButtonPressCount !== undefined) {
                document.getElementById("program-button-count").innerText = data.programStartButtonPressCount;
            }
            if (data.resetProgramButtonPressed !== undefined) {
                updateIndicator("reset-program-button-ind", data.resetProgramButtonPressed);
                document.getElementById("reset-program-button-state").innerText = data.resetProgramButtonPressed ? "Pressed" : "Released";
            }
            if (data.resetProgramButtonPressCount !== undefined) {
                document.getElementById("reset-program-button-count").innerText = data.resetProgramButtonPressCount;
            }
        }

        function updateProgramRelayTelemetry(data) {
            updateProgramRelayStatus(1, data.relayR1Active, data.relayR1Pin);
            updateProgramRelayStatus(2, data.relayR2Active, data.relayR2Pin);
            updateProgramRelayStatus(3, data.relayR3Active, data.relayR3Pin);
        }

        function updateProgramRelayStatus(relayId, active, pin) {
            if (active === undefined) return;
            updateProgramRelayTarget("relay-r" + relayId, active, pin);
            updateProgramRelayTarget("dashboard-relay-r" + relayId, active, pin);
        }

        function updateProgramRelayTarget(prefix, active, pin) {
            updateIndicator(prefix + "-ind", active);
            const state = document.getElementById(prefix + "-state");
            const pinLabel = document.getElementById(prefix + "-pin");
            if (state) state.innerText = active ? "On" : "Off";
            if (pinLabel && pin !== undefined) pinLabel.innerText = pin;
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
            document.getElementById("m" + id + "-cfg-angle").value = cfg.stepAngle;
            document.getElementById("m" + id + "-cfg-usteps").value = cfg.microsteps;
            document.getElementById("m" + id + "-cfg-drive").value = cfg.driveType;
            document.getElementById("m" + id + "-cfg-pitch").value = cfg.leadScrewPitch;
            document.getElementById("m" + id + "-cfg-beltpitch").value = cfg.beltPitch;
            document.getElementById("m" + id + "-cfg-teeth").value = cfg.pulleyTeeth;
            const stepsPerMm = calculateStepsPerMm(cfg.stepAngle, cfg.microsteps, cfg.driveType,
                cfg.leadScrewPitch, cfg.beltPitch, cfg.pulleyTeeth);
            document.getElementById("m" + id + "-cfg-min").value = formatLimitMillimeters(cfg.minPosition / stepsPerMm);
            document.getElementById("m" + id + "-cfg-max").value = formatLimitMillimeters(cfg.maxPosition / stepsPerMm);
            document.getElementById("m" + id + "-cfg-unit").value = cfg.unitType;
            document.getElementById("m" + id + "-cfg-home-fast").value = cfg.homeFastSpeed;
            document.getElementById("m" + id + "-cfg-home-slow").value = cfg.homeSlowSpeed;
            document.getElementById("m" + id + "-cfg-home-accel").value = cfg.homeAcceleration;
            document.getElementById("m" + id + "-cfg-home-backoff").value = cfg.homeBackoffDistance;
            document.getElementById("m" + id + "-cfg-home-max-release").value = cfg.homeMaxReleaseDistance;
            document.getElementById("m" + id + "-cfg-home-settle").value = cfg.homeDirectionSettleMs;
            document.getElementById("m" + id + "-cfg-home-debounce").value = cfg.homeSwitchDebounceMs;
            document.getElementById("m" + id + "-cfg-home-timeout").value = cfg.homeSearchTimeoutSec;
            updateHomeUnitLabels(id);
            toggleDriveInputs(id);
            updateLimitStepInfo(id);
        }

        function calculateStepsPerMm(stepAngle, microsteps, driveType, leadScrewPitch, beltPitch, pulleyTeeth) {
            const angle = Number(stepAngle);
            const usteps = Number(microsteps);
            const travelPerRev = Number(driveType) === 0
                ? Number(leadScrewPitch)
                : Number(beltPitch) * Number(pulleyTeeth);
            if (!Number.isFinite(angle) || angle <= 0 ||
                !Number.isFinite(usteps) || usteps <= 0 ||
                !Number.isFinite(travelPerRev) || travelPerRev <= 0) return NaN;
            return (360 / angle) * usteps / travelPerRev;
        }

        function getStepsPerMmFromInputs(id) {
            return calculateStepsPerMm(
                document.getElementById("m" + id + "-cfg-angle").value,
                document.getElementById("m" + id + "-cfg-usteps").value,
                document.getElementById("m" + id + "-cfg-drive").value,
                document.getElementById("m" + id + "-cfg-pitch").value,
                document.getElementById("m" + id + "-cfg-beltpitch").value,
                document.getElementById("m" + id + "-cfg-teeth").value
            );
        }

        function formatLimitMillimeters(value) {
            if (!Number.isFinite(value)) return "";
            return String(Math.round(value * 10000) / 10000);
        }

        function updateLimitStepInfo(id) {
            const stepsPerMm = getStepsPerMmFromInputs(id);
            for (const bound of ["min", "max"]) {
                const millimeters = parseFloat(document.getElementById("m" + id + "-cfg-" + bound).value);
                const info = document.getElementById("m" + id + "-cfg-" + bound + "-steps");
                info.innerText = Number.isFinite(stepsPerMm) && Number.isFinite(millimeters)
                    ? "Calculated: " + Math.round(millimeters * stepsPerMm) + " steps (" + stepsPerMm.toFixed(3) + " steps/mm)"
                    : "Calculated: check the mechanical settings";
            }
        }

        function updateHomeUnitLabels(id) {
            const unit = getUnitName(parseInt(document.getElementById("m" + id + "-cfg-unit").value, 10));
            document.getElementById("m" + id + "-home-unit-speed").innerText = unit + "/s";
            document.getElementById("m" + id + "-home-unit-slow").innerText = unit + "/s";
            document.getElementById("m" + id + "-home-unit-accel").innerText = unit + "/s2";
            document.getElementById("m" + id + "-home-unit-backoff").innerText = unit;
            document.getElementById("m" + id + "-home-unit-release").innerText = unit;
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

        const jogSteps = { 1: 1, 2: 1 };

        function setJogStep(motorId, amount) {
            jogSteps[motorId] = amount;
            document.querySelectorAll('[data-jog-motor="' + motorId + '"]').forEach(button => {
                const selected = parseFloat(button.dataset.jogStep) === amount;
                button.classList.toggle("active", selected);
                button.setAttribute("aria-pressed", selected ? "true" : "false");
            });
            logConsole("Motor " + motorId + " manual move step set to " + amount + ".");
        }

        function jogMotorByStep(motorId, direction) {
            jogMotor(motorId, direction * jogSteps[motorId]);
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
            programTexts[selectedProgramSlot] = editor.value;
            localStorage.setItem("motionProgramText" + selectedProgramSlot, editor.value);
            logConsole("Program " + selectedProgramSlot + " sent to ESP32.");
            sendWsMsg({ cmd: "program_start", slot: selectedProgramSlot, program: editor.value });
        }

        function runResetProgram() {
            const editor = document.getElementById("reset-program-editor");
            const program = editor.value.trim();
            if (!program.length) {
                logConsole("Reset Program is empty.", "error");
                return;
            }
            localStorage.setItem("resetProgramText", editor.value);
            logConsole("Reset Program sent to ESP32.");
            sendWsMsg({ cmd: "reset_program_start", program: editor.value });
        }

        function programEditorChanged() {
            const editor = document.getElementById("program-editor");
            const editedSlot = selectedProgramSlot;
            const editedText = editor.value;
            programTexts[editedSlot] = editedText;
            localStorage.setItem("motionProgramText" + editedSlot, editedText);
            updateProgramLineNumbers();
            if (programSaveTimer) clearTimeout(programSaveTimer);
            programSaveTimer = setTimeout(function() {
                syncProgramTextToDevice(false, editedSlot, editedText);
            }, 800);
        }

        function selectSequentialProgram(programSlot, notifyDevice = true) {
            programSlot = Number(programSlot) === 2 ? 2 : 1;
            if (programIsRunning && programSlot !== selectedProgramSlot) {
                logConsole("Program selection is locked while a program is running.", "warn");
                return;
            }

            const editor = document.getElementById("program-editor");
            if (editor && programSlot !== selectedProgramSlot) {
                programTexts[selectedProgramSlot] = editor.value;
                localStorage.setItem("motionProgramText" + selectedProgramSlot, editor.value);
                syncProgramTextToDevice(false, selectedProgramSlot, editor.value);
                selectedProgramSlot = programSlot;
                editor.value = programTexts[selectedProgramSlot] || "";
                editor.scrollTop = 0;
                activeProgramLineNumber = 0;
            }

            updateProgramSelector();
            updateProgramLineNumbers();
            if (notifyDevice) {
                sendWsMsg({ cmd: "program_select", slot: selectedProgramSlot });
                logConsole("Program " + selectedProgramSlot + " selected.");
            }
        }

        function updateProgramSelector() {
            for (let programSlot = 1; programSlot <= 2; programSlot++) {
                const choice = document.getElementById("program-choice-" + programSlot);
                if (!choice) continue;
                choice.classList.toggle("active", programSlot === selectedProgramSlot);
                choice.disabled = programIsRunning && programSlot !== selectedProgramSlot;
                choice.setAttribute("aria-pressed", programSlot === selectedProgramSlot ? "true" : "false");
            }
            document.getElementById("program-editor-label").innerText = "Program " + selectedProgramSlot + " Text";
            document.getElementById("program-saved-label").innerText = "Program " + selectedProgramSlot + " Saved";
        }

        function resetProgramEditorChanged() {
            const editor = document.getElementById("reset-program-editor");
            localStorage.setItem("resetProgramText", editor.value);
            updateProgramLineNumbers();
            if (resetProgramSaveTimer) clearTimeout(resetProgramSaveTimer);
            resetProgramSaveTimer = setTimeout(function() {
                syncResetProgramTextToDevice(false);
            }, 800);
        }

        function updateProgramLineNumbers() {
            updateProgramLineNumbersFor("program");
            updateProgramLineNumbersFor("reset");
        }

        function updateProgramLineNumbersFor(editorType) {
            const editor = document.getElementById(editorType === "reset" ? "reset-program-editor" : "program-editor");
            const gutter = document.getElementById(editorType === "reset" ? "reset-program-line-numbers" : "program-line-numbers");
            if (!editor || !gutter) return;

            const lineCount = editor.value.split("\n").length;
            let numbers = "";
            for (let lineNumber = 1; lineNumber <= lineCount; lineNumber++) {
                const activeClass = editorType === activeProgramEditor && lineNumber === activeProgramLineNumber ? " active" : "";
                numbers += '<div class="program-line-number' + activeClass + '">' + lineNumber + "</div>";
            }
            gutter.innerHTML = numbers;
            syncProgramEditorScroll(editorType);
        }

        function syncProgramEditorScroll(editorType = "program") {
            const editor = document.getElementById(editorType === "reset" ? "reset-program-editor" : "program-editor");
            const gutter = document.getElementById(editorType === "reset" ? "reset-program-line-numbers" : "program-line-numbers");
            if (editor && gutter) gutter.scrollTop = editor.scrollTop;
        }

        function setActiveProgramLine(lineNumber) {
            if (lineNumber === activeProgramLineNumber) {
                updateProgramLineNumbers();
                return;
            }
            activeProgramLineNumber = lineNumber;
            updateProgramLineNumbers();

            if (lineNumber <= 0) return;
            const editor = document.getElementById(activeProgramEditor === "reset" ? "reset-program-editor" : "program-editor");
            const lineHeight = parseFloat(window.getComputedStyle(editor).lineHeight) || 21;
            const lineTop = (lineNumber - 1) * lineHeight;
            const lineBottom = lineTop + lineHeight;
            if (lineTop < editor.scrollTop || lineBottom > editor.scrollTop + editor.clientHeight) {
                editor.scrollTop = Math.max(0, lineTop - editor.clientHeight / 2 + lineHeight / 2);
                syncProgramEditorScroll(activeProgramEditor);
            }
        }

        function syncProgramTextToDevice(showLog, programSlot = selectedProgramSlot, programText = null) {
            const editor = document.getElementById("program-editor");
            if (!editor || !ws || ws.readyState !== WebSocket.OPEN) return;
            const text = programText === null ? editor.value : programText;
            sendWsMsg({ cmd: "program_save", slot: programSlot, program: text });
            if (showLog) logConsole("Program " + programSlot + " text saved to ESP32.");
        }

        function syncResetProgramTextToDevice(showLog) {
            const editor = document.getElementById("reset-program-editor");
            if (!editor || !ws || ws.readyState !== WebSocket.OPEN) return;
            sendWsMsg({ cmd: "reset_program_save", program: editor.value });
            if (showLog) logConsole("Reset Program text saved to ESP32.");
        }

        function stopProgram() {
            logConsole("Program stop requested.", "warn");
            sendWsMsg({ cmd: "program_stop" });
        }

        function triggerRelay(relayId) {
            const input = document.getElementById("r" + relayId + "-duration");
            let durationMs = parseInt(input.value, 10);
            if (isNaN(durationMs) || durationMs < 1) durationMs = 1;
            if (durationMs > 86400000) durationMs = 86400000;
            input.value = durationMs;
            logConsole("Gen.Out" + relayId + " triggered for " + durationMs + " ms.");
            sendWsMsg({ cmd: "relay_trigger", relay: relayId, durationMs: durationMs });
        }

        function turnRelayOff(relayId) {
            logConsole("Gen.Out" + relayId + " manual off requested.");
            sendWsMsg({ cmd: "relay_off", relay: relayId });
        }

        function setProgramRelay(relayId, state) {
            logConsole("Manual test: R" + relayId + (state ? " ON requested." : " OFF requested."));
            sendWsMsg({ cmd: "program_relay_set", relay: relayId, state: !!state });
        }

        function rebootESP() {
            if (confirm("Restart the ESP32 now?")) {
                logConsole("Restart command sent.", "warn");
                sendWsMsg({ cmd: "reboot" });
            }
        }

        function saveMotorSettings() {
            const m1 = readMotorConfig(1);
            const m2 = readMotorConfig(2);
            for (const item of [{ id: 1, cfg: m1 }, { id: 2, cfg: m2 }]) {
                if (!Number.isFinite(item.cfg.minPosition) || !Number.isFinite(item.cfg.maxPosition)) {
                    alert("Motor " + item.id + ": Enter valid mechanical settings and software limits in millimeters.");
                    return;
                }
                if (item.cfg.minPosition > item.cfg.maxPosition) {
                    alert("Motor " + item.id + ": Software Minimum Limit cannot be greater than Software Maximum Limit.");
                    return;
                }
                if (item.cfg.homeBackoffDistance <= 0 || item.cfg.homeMaxReleaseDistance < item.cfg.homeBackoffDistance) {
                    alert("Motor " + item.id + ": Maximum Switch Release Travel must be equal to or greater than Extra Clearance.");
                    return;
                }
            }
            const cfg = {
                cmd: "save_config",
                m1: m1,
                m2: m2,
                sys: {
                    timeoutActive: document.getElementById("sys-cfg-timeout-active").value === "1",
                    timeoutDuration: parseInt(document.getElementById("sys-cfg-timeout-duration").value, 10)
                }
            };
            logConsole("Sending motor settings.");
            sendWsMsg(cfg);
        }

        function readMotorConfig(id) {
            const stepAngle = parseFloat(document.getElementById("m" + id + "-cfg-angle").value);
            const microsteps = parseInt(document.getElementById("m" + id + "-cfg-usteps").value, 10);
            const driveType = parseInt(document.getElementById("m" + id + "-cfg-drive").value, 10);
            const leadScrewPitch = parseFloat(document.getElementById("m" + id + "-cfg-pitch").value);
            const beltPitch = parseFloat(document.getElementById("m" + id + "-cfg-beltpitch").value);
            const pulleyTeeth = parseInt(document.getElementById("m" + id + "-cfg-teeth").value, 10);
            const stepsPerMm = calculateStepsPerMm(stepAngle, microsteps, driveType,
                leadScrewPitch, beltPitch, pulleyTeeth);
            const minMillimeters = parseFloat(document.getElementById("m" + id + "-cfg-min").value);
            const maxMillimeters = parseFloat(document.getElementById("m" + id + "-cfg-max").value);
            return {
                maxSpeed: parseFloat(document.getElementById("m" + id + "-cfg-speed").value),
                acceleration: parseFloat(document.getElementById("m" + id + "-cfg-accel").value),
                minPosition: Math.round(minMillimeters * stepsPerMm),
                maxPosition: Math.round(maxMillimeters * stepsPerMm),
                stepAngle: stepAngle,
                microsteps: microsteps,
                driveType: driveType,
                leadScrewPitch: leadScrewPitch,
                beltPitch: beltPitch,
                pulleyTeeth: pulleyTeeth,
                unitType: parseInt(document.getElementById("m" + id + "-cfg-unit").value, 10),
                homeFastSpeed: parseFloat(document.getElementById("m" + id + "-cfg-home-fast").value),
                homeSlowSpeed: parseFloat(document.getElementById("m" + id + "-cfg-home-slow").value),
                homeAcceleration: parseFloat(document.getElementById("m" + id + "-cfg-home-accel").value),
                homeBackoffDistance: parseFloat(document.getElementById("m" + id + "-cfg-home-backoff").value),
                homeMaxReleaseDistance: parseFloat(document.getElementById("m" + id + "-cfg-home-max-release").value),
                homeDirectionSettleMs: parseInt(document.getElementById("m" + id + "-cfg-home-settle").value, 10),
                homeSwitchDebounceMs: parseInt(document.getElementById("m" + id + "-cfg-home-debounce").value, 10),
                homeSearchTimeoutSec: parseFloat(document.getElementById("m" + id + "-cfg-home-timeout").value)
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
            programTexts[1] = localStorage.getItem("motionProgramText1") || localStorage.getItem("motionProgramText") || document.getElementById("program-editor").value;
            programTexts[2] = localStorage.getItem("motionProgramText2") || "";
            document.getElementById("program-editor").value = programTexts[1];
            const savedResetProgram = localStorage.getItem("resetProgramText");
            if (savedResetProgram) document.getElementById("reset-program-editor").value = savedResetProgram;
            updateProgramSelector();
            updateProgramLineNumbers();
            initWebSocket();
        });
    </script>
</body>
</html>
)rawhtml";

#endif // INDEX_HTML_H
