/*
  parking_sensor.ino
  Smart Parking Assistant — ESP8266 NodeMCU Firmware

  This sketch reads HC-SR04 ultrasonic sensors connected to the NodeMCU,
  determines if each slot is occupied or free, and sends the results to
  the backend server as a JSON payload every POST_INTERVAL_MS milliseconds.

  Wire each sensor as described in hardware/wiring_diagram.md before uploading.

  Dependencies:
    - ESP8266WiFi (comes with ESP8266 board package)
    - ESP8266HTTPClient (comes with ESP8266 board package)
    - ArduinoJson (install via Library Manager, version 6.x)
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "config.h"

// -----------------------------------------------------------------------
// Sensor pin definitions
// TRIG and ECHO pin pairs for each slot.
// Adjust these to match your actual wiring.
// -----------------------------------------------------------------------

const int TRIG_PINS[SLOT_COUNT] = {D1, D3, D5, D7, D0, D2};
const int ECHO_PINS[SLOT_COUNT] = {D2, D4, D6, D8, D6, D8};

// Wait for Wi-Fi connection on startup
void connectWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect. Will retry in loop.");
  }
}

// Measure distance using HC-SR04
// Returns distance in centimeters, or -1 if measurement times out
float measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;

  float distance = (duration * 0.034) / 2.0;
  return distance;
}

// Send current slot states to the backend server
void postSensorData(bool occupied[]) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi not connected. Skipping post.");
    return;
  }

  WiFiClient client;
  HTTPClient http;

  String url = String(SERVER_URL) + POST_ENDPOINT;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");

  // Build JSON payload
  StaticJsonDocument<512> doc;
  doc["zone"]        = ZONE_ID;
  doc["vehicleType"] = VEHICLE_TYPE;
  doc["timestamp"]   = millis();

  JsonArray slots = doc.createNestedArray("slots");
  for (int i = 0; i < SLOT_COUNT; i++) {
    JsonObject slot = slots.createNestedObject();
    slot["id"]       = String(ZONE_ID) + String(i + 1);
    slot["occupied"] = occupied[i];
  }

  String payload;
  serializeJson(doc, payload);

  Serial.println("Sending: " + payload);

  int responseCode = http.POST(payload);
  if (responseCode > 0) {
    Serial.println("Server response: " + String(responseCode));
  } else {
    Serial.println("POST failed: " + http.errorToString(responseCode));
  }

  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialize sensor pins
  for (int i = 0; i < SLOT_COUNT; i++) {
    pinMode(TRIG_PINS[i], OUTPUT);
    pinMode(ECHO_PINS[i], INPUT);
  }

  connectWiFi();
}

unsigned long lastPost = 0;

void loop() {
  // Reconnect if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost. Reconnecting...");
    connectWiFi();
  }

  unsigned long now = millis();
  if (now - lastPost >= POST_INTERVAL_MS) {
    lastPost = now;

    bool occupied[SLOT_COUNT];

    for (int i = 0; i < SLOT_COUNT; i++) {
      float dist = measureDistance(TRIG_PINS[i], ECHO_PINS[i]);

      if (dist < 0) {
        // Sensor error or timeout — keep previous state
        Serial.print("Slot ");
        Serial.print(i + 1);
        Serial.println(": sensor timeout");
        occupied[i] = false;
      } else {
        occupied[i] = (dist < OCCUPIED_THRESHOLD_CM);
        Serial.print("Slot ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(dist);
        Serial.print(" cm — ");
        Serial.println(occupied[i] ? "OCCUPIED" : "FREE");
      }
    }

    postSensorData(occupied);
  }
}
