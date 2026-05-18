/*
  server.js — Smart Parking Assistant Backend
  
  This is a lightweight Express server that:
  1. Receives sensor data from ESP8266 boards via POST /api/update
  2. Stores the current slot state in memory
  3. Serves the current state to the frontend via GET /api/status
  4. Serves the frontend HTML file at the root URL (what users see when they scan the QR code)

  Run with:  node server.js
  Requires:  npm install
*/

require('dotenv').config();
const express = require('express');
const cors    = require('cors');
const path    = require('path');

const app  = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

// Serve static frontend files from the ../frontend directory
app.use(express.static(path.join(__dirname, '..', 'frontend')));

// ---------------------------------------------------------------------------
// In-memory state
// This holds all slot data received from ESP8266 boards.
// Key: slot ID (e.g. "A1", "B3"), Value: slot object
// ---------------------------------------------------------------------------
const slotState = {};

// Initialize with default slots so the frontend shows something before sensors connect
function initDefaultSlots() {
  const zones = [
    { zone: 'A', type: '4W', count: 6 },
    { zone: 'B', type: '2W', count: 10 }
  ];

  zones.forEach(({ zone, type, count }) => {
    for (let i = 1; i <= count; i++) {
      const id = `${zone}${i}`;
      slotState[id] = {
        id,
        zone,
        vehicleType: type,
        occupied: false,
        lastUpdate: new Date().toISOString()
      };
    }
  });

  console.log(`Initialized ${Object.keys(slotState).length} default slots.`);
}

initDefaultSlots();

// ---------------------------------------------------------------------------
// POST /api/update
// Called by ESP8266 firmware every few seconds.
// Body shape: { zone, vehicleType, timestamp, slots: [ { id, occupied }, ... ] }
// ---------------------------------------------------------------------------
app.post('/api/update', (req, res) => {
  const { zone, vehicleType, slots } = req.body;

  if (!zone || !slots || !Array.isArray(slots)) {
    return res.status(400).json({ error: 'Invalid payload. Expected zone and slots array.' });
  }

  const now = new Date().toISOString();

  slots.forEach(sensor => {
    slotState[sensor.id] = {
      id:          sensor.id,
      zone:        zone,
      vehicleType: vehicleType,
      occupied:    sensor.occupied,
      lastUpdate:  now
    };
  });

  console.log(`[${now}] Updated ${slots.length} slots in zone ${zone} (${vehicleType})`);
  res.json({ ok: true, updated: slots.length });
});

// ---------------------------------------------------------------------------
// GET /api/status
// Called by the frontend to get the current state of all slots.
// Returns summary counts plus the full slot list.
// ---------------------------------------------------------------------------
app.get('/api/status', (req, res) => {
  const allSlots = Object.values(slotState);

  const fourWheeler = allSlots.filter(s => s.vehicleType === '4W');
  const twoWheeler  = allSlots.filter(s => s.vehicleType === '2W');

  res.json({
    timestamp: new Date().toISOString(),
    summary: {
      fourWheeler: {
        total: fourWheeler.length,
        free:  fourWheeler.filter(s => !s.occupied).length,
        occupied: fourWheeler.filter(s => s.occupied).length
      },
      twoWheeler: {
        total: twoWheeler.length,
        free:  twoWheeler.filter(s => !s.occupied).length,
        occupied: twoWheeler.filter(s => s.occupied).length
      }
    },
    slots: allSlots
  });
});

// ---------------------------------------------------------------------------
// GET / — fallback to frontend index.html
// ---------------------------------------------------------------------------
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, '..', 'frontend', 'index.html'));
});

app.listen(PORT, () => {
  console.log(`Smart Parking server running on port ${PORT}`);
  console.log(`Frontend: http://localhost:${PORT}`);
  console.log(`API:      http://localhost:${PORT}/api/status`);
});
