// config.h - Smart Parking Assistant
// Fill in your credentials before uploading.
// This file is listed in .gitignore — do NOT commit it with real credentials.

#ifndef CONFIG_H
#define CONFIG_H

// Your Wi-Fi network name
#define WIFI_SSID "YourNetworkName"

// Your Wi-Fi password
#define WIFI_PASSWORD "YourPassword"

// IP address and port of the machine running backend/server.js
// Example: "http://192.168.1.100:3000"
#define SERVER_URL "http://192.168.1.100:3000"

// Endpoint to POST sensor data to
#define POST_ENDPOINT "/api/update"

// Zone identifier for this board (used when multiple ESP8266 boards are deployed)
// "A" = Zone A, "B" = Zone B, etc.
#define ZONE_ID "A"

// Vehicle type this board monitors: "2W" for two-wheelers, "4W" for four-wheelers
#define VEHICLE_TYPE "4W"

// Number of slots this board manages (max 8 without a multiplexer)
#define SLOT_COUNT 6

// Distance threshold in centimeters — anything below this means a vehicle is present
#define OCCUPIED_THRESHOLD_CM 20

// How often to send data to the server, in milliseconds
#define POST_INTERVAL_MS 2000

#endif
