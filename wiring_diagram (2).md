# Wiring Diagram — HC-SR04 to ESP8266 NodeMCU

## Sensor Pin Assignment

Each HC-SR04 sensor has four pins: VCC, GND, TRIG, and ECHO.

VCC connects to the 5V pin on NodeMCU (or 3.3V with a voltage divider on ECHO — see note below).
GND connects to any GND pin on NodeMCU.
TRIG is an input — connect to a NodeMCU digital output GPIO.
ECHO is an output — it returns a 5V signal on most HC-SR04s.

## IMPORTANT — Voltage Level on ECHO Pin

The ESP8266 GPIO pins are 3.3V tolerant only. The HC-SR04 ECHO line outputs 5V.
Connecting 5V directly to an ESP8266 GPIO can damage the chip over time.

Simple fix: use a voltage divider on the ECHO line.

    HC-SR04 ECHO ---> [1k ohm] ---> GPIO pin
                                |
                              [2k ohm]
                                |
                               GND

This drops the 5V signal to approximately 3.3V before it reaches the ESP8266.

Alternatively, use the HY-SRF05 sensor which supports 3.3V operation natively.
Or use a logic level converter module between the sensor and the board.

## Pin Mapping (Default — matches firmware config.h)

Slot 1 (Zone A):  TRIG = D1, ECHO = D2
Slot 2 (Zone A):  TRIG = D3, ECHO = D4
Slot 3 (Zone A):  TRIG = D5, ECHO = D6
Slot 4 (Zone A):  TRIG = D7, ECHO = D8
Slot 5 (Zone A):  TRIG = D0, ECHO = D6 (shared via multiplexer or timed)
Slot 6 (Zone A):  TRIG = D2, ECHO = D8 (shared via multiplexer or timed)

Note: The NodeMCU does not have enough unique GPIO pairs for more than 4-5 sensors without a multiplexer.
For 6-12 sensors on one board, use a CD74HC4067 16-channel analog/digital multiplexer.

## Powering Multiple Sensors

Each HC-SR04 draws about 15mA. For 6 sensors, that is 90mA total.
The NodeMCU 5V pin can source this from USB power comfortably.
For more than 8 sensors, use a dedicated 5V 1A wall adapter shared across all sensor VCC pins,
with all GNDs tied together including the NodeMCU GND.

## Physical Layout Recommendation

Mount each sensor centered above the parking slot, 1.5 to 2.5 meters above ground level.
Angle it 0 degrees (straight down) for overhead mounting.
For side-entry detection, mount at bumper height (30-40 cm) aimed horizontally across the slot entrance.
Keep sensor faces clean and avoid mounting where direct sunlight hits the sensor face at low angles.

## Running Cables

Use standard 4-core alarm cable or CAT5 for long sensor runs.
Max recommended cable length before signal degradation: 3 to 4 meters without shielding.
For longer runs, use a sensor driver IC like the LV-MaxSonar series or add a signal repeater.
