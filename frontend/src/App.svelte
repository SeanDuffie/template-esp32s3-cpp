<script>
    import { onMount } from 'svelte';

    // Mock Live Data
    let liveData = {
        temperature: 72.4, // °F
        soilMoisture: 45,  // %
    };

    // Mock Cycle Data
    let cycleData = {
        startDate: "2026-03-01",
        daysElapsed: 25,
        estimatedEndDate: "2026-05-01"
    };

    // Mock Historical Stats (7-Day)
    let stats = {
        temp: { high: 78.1, low: 65.2, avg: 71.5, targetMin: 68, targetMax: 75, exceededCount: 3 },
        moisture: { high: 80, low: 30, avg: 55, targetMin: 40, targetMax: 70, exceededCount: 1 }
    };

    // System States
    let isUpdating = false;
    let wifiSSID = "";
    let wifiPass = "";
    let wifiStatusMessage = "";

    async function handleUpdate() {
        isUpdating = true;
        try {
            // Replace with actual ESP32 API endpoint
            const res = await fetch('/api/update', { method: 'POST' });
            if (res.ok) alert("Firmware update triggered. Device will reboot.");
            else alert("Update failed to start.");
        } catch (e) {
            alert("Network error.");
        }
        isUpdating = false;
    }

    async function handleWifiUpdate() {
        if (!wifiSSID) {
            wifiStatusMessage = "SSID cannot be blank.";
            return;
        }
        wifiStatusMessage = "Sending credentials to device...";
        try {
            // Replace with actual ESP32 API endpoint
            const res = await fetch('/api/wifi', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ ssid: wifiSSID, password: wifiPass })
            });
            if (res.ok) wifiStatusMessage = "Credentials saved. Rebooting to apply...";
            else wifiStatusMessage = "Failed to update credentials.";
        } catch (e) {
            wifiStatusMessage = "Network error.";
        }
    }
</script>

<main class="dashboard">
    <header>
        <h1>Sensor Node Dashboard</h1>
        <span class="status-indicator">● Online</span>
    </header>

    <div class="grid-container">
        <section class="card">
            <h2>Current Readings</h2>
            <div class="reading">
                <span class="label">Temperature</span>
                <span class="value">{liveData.temperature}°F</span>
            </div>
            <div class="reading">
                <span class="label">Soil Moisture</span>
                <span class="value">{liveData.soilMoisture}%</span>
            </div>
        </section>

        <section class="card">
            <h2>Cycle Status</h2>
            <div class="reading">
                <span class="label">Start Date</span>
                <span class="value">{cycleData.startDate}</span>
            </div>
            <div class="reading">
                <span class="label">Days Elapsed</span>
                <span class="value">{cycleData.daysElapsed}</span>
            </div>
            <div class="reading">
                <span class="label">Est. End Date</span>
                <span class="value">{cycleData.estimatedEndDate}</span>
            </div>
        </section>

        <section class="card full-width">
            <h2>7-Day Historical Statistics</h2>
            <table class="stats-table">
                <thead>
                    <tr>
                        <th>Metric</th>
                        <th>High</th>
                        <th>Low</th>
                        <th>Average</th>
                        <th>Target Bounds</th>
                        <th>Target Exceeded</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td>Temperature</td>
                        <td>{stats.temp.high}°F</td>
                        <td>{stats.temp.low}°F</td>
                        <td>{stats.temp.avg}°F</td>
                        <td>{stats.temp.targetMin} - {stats.temp.targetMax}°F</td>
                        <td class="{stats.temp.exceededCount > 0 ? 'alert' : ''}">{stats.temp.exceededCount} times</td>
                    </tr>
                    <tr>
                        <td>Soil Moisture</td>
                        <td>{stats.moisture.high}%</td>
                        <td>{stats.moisture.low}%</td>
                        <td>{stats.moisture.avg}%</td>
                        <td>{stats.moisture.targetMin} - {stats.moisture.targetMax}%</td>
                        <td class="{stats.moisture.exceededCount > 0 ? 'alert' : ''}">{stats.moisture.exceededCount} times</td>
                    </tr>
                </tbody>
            </table>
        </section>

        <section class="card full-width admin-panel">
            <h2>System Administration</h2>
            
            <div class="admin-grid">
                <div class="firmware-section">
                    <h3>Firmware</h3>
                    <p>Pull latest firmware from the local server.</p>
                    <button on:click={handleUpdate} disabled={isUpdating}>
                        {isUpdating ? 'Downloading...' : 'Pull Update'}
                    </button>
                </div>

                <div class="wifi-section">
                    <h3>Network Configuration</h3>
                    <p>Update network credentials. Device will reboot upon saving.</p>
                    <input type="text" placeholder="New SSID" bind:value={wifiSSID} />
                    <input type="password" placeholder="New Password" bind:value={wifiPass} />
                    <button on:click={handleWifiUpdate}>Save & Reboot</button>
                    {#if wifiStatusMessage}
                        <p class="status-msg">{wifiStatusMessage}</p>
                    {/if}
                </div>
            </div>
        </section>
    </div>
</main>

<style>
    :global(body) {
        font-family: system-ui, -apple-system, sans-serif;
        background-color: #f4f4f5;
        color: #18181b;
        margin: 0;
        padding: 20px;
    }

    .dashboard {
        max-width: 1000px;
        margin: 0 auto;
    }

    header {
        display: flex;
        justify-content: space-between;
        align-items: center;
        margin-bottom: 20px;
    }

    .status-indicator {
        color: #16a34a;
        font-weight: bold;
    }

    .grid-container {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 20px;
    }

    .card {
        background: white;
        padding: 20px;
        border-radius: 8px;
        box-shadow: 0 1px 3px rgba(0,0,0,0.1);
    }

    .full-width {
        grid-column: 1 / -1;
    }

    h2 {
        margin-top: 0;
        font-size: 1.2rem;
        border-bottom: 1px solid #e4e4e7;
        padding-bottom: 10px;
    }

    .reading {
        display: flex;
        justify-content: space-between;
        padding: 10px 0;
        border-bottom: 1px solid #f4f4f5;
    }

    .reading .value {
        font-weight: bold;
        font-size: 1.1rem;
    }

    .stats-table {
        width: 100%;
        border-collapse: collapse;
        text-align: left;
    }

    .stats-table th, .stats-table td {
        padding: 12px;
        border-bottom: 1px solid #e4e4e7;
    }

    .alert {
        color: #dc2626;
        font-weight: bold;
    }

    .admin-grid {
        display: grid;
        grid-template-columns: 1fr 1fr;
        gap: 20px;
    }

    input {
        display: block;
        width: 100%;
        padding: 8px;
        margin-bottom: 10px;
        border: 1px solid #d4d4d8;
        border-radius: 4px;
        box-sizing: border-box;
    }

    button {
        background: #2563eb;
        color: white;
        border: none;
        padding: 10px 15px;
        border-radius: 4px;
        cursor: pointer;
        font-weight: bold;
    }

    button:disabled {
        background: #93c5fd;
        cursor: not-allowed;
    }

    .status-msg {
        font-size: 0.9rem;
        color: #52525b;
        margin-top: 10px;
    }
</style>