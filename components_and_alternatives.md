# Component Reference — Smart Parking Assistant

This document covers every hardware component used in the system with photos, full specifications,
sourcing notes, and a ranked list of alternatives for each part. Alternatives are grouped by use
case: budget builds, outdoor environments, higher accuracy, and easier wiring.

Contact for questions: carmodbhai@gmail.com

---

## How to Read This Document

Each component entry has four parts. First, a photo reference with links to product images so you
know exactly what to look for when buying. Second, a specification table. Third, why this specific
part was chosen over the alternatives. Fourth, a ranked list of drop-in replacements with their
trade-offs clearly spelled out.

---

## Component 01 — Main Microcontroller

### ESP8266 NodeMCU v3 (CP2102)

Photo references:
- Product image: https://robu.in/wp-content/uploads/2020/01/NodeMCU-ESP8266-Development-Board-CH340-1.jpg
- Pinout diagram: https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2019/07/ESP8266-NodeMCU-kit-12-E-pinout-gpio-pin.png

| Specification       | Value                          |
|---------------------|--------------------------------|
| Processor           | Tensilica L106 32-bit          |
| Clock Speed         | 80 MHz (overclockable to 160)  |
| Flash Memory        | 4 MB                           |
| RAM                 | 80 KB                          |
| Wi-Fi               | Built-in 802.11 b/g/n          |
| GPIO Pins           | 11 usable                      |
| Operating Voltage   | 3.3V logic, powered via 5V USB |
| USB-Serial Chip     | CP2102 (recommended over CH340)|
| Programming         | Arduino IDE, MicroPython, Lua  |
| Price               | Rs. 200-250                    |
| Availability        | Everywhere — Amazon, Robu.in, local |

Why this was chosen: The ESP8266 has Wi-Fi built in. Every alternative microcontroller that
does not have Wi-Fi (Arduino Uno, Nano, ATmega328 boards) needs a separate Wi-Fi module
costing Rs. 200-400 extra. The NodeMCU bundles processor, Wi-Fi, USB programming port,
and voltage regulator on a single board. For parking lot sensor deployment, where each
zone controller needs to push data to the server wirelessly, this is the most cost-efficient
option available.

---

### Alternative Boards

---

#### Alternative 01 — ESP32 DevKit (Recommended Upgrade)

Photo references:
- Product image: https://robu.in/wp-content/uploads/2019/12/NodeMCU-ESP32-Development-Board-1.jpg
- Pinout: https://mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png

| Specification       | Value                              |
|---------------------|------------------------------------|
| Processor           | Dual-core Xtensa LX6 240 MHz       |
| RAM                 | 520 KB                             |
| Wi-Fi               | Built-in 802.11 b/g/n              |
| Bluetooth           | BLE 4.2 + Classic                  |
| GPIO Pins           | 30+ usable                         |
| ADC Channels        | 18 (vs 1 on ESP8266)               |
| Price               | Rs. 350-450                        |

Best for: Installations where you need more GPIO pins (so fewer multiplexers), want Bluetooth
for a handheld configuration tool, or plan to add a small display or camera later. The extra
RAM also helps if you add MQTT, TLS, or OTA updates to the firmware. The ESP32 runs the same
Arduino IDE code as ESP8266 with very minor changes.

Trade-off: Rs. 150 more expensive per board. No meaningful disadvantage for this use case.

---

#### Alternative 02 — Arduino Nano + ESP8266-01 Wi-Fi Module

Photo references:
- Arduino Nano: https://store.arduino.cc/cdn/shop/products/A000005_01.iso.jpg
- ESP-01 module: https://robu.in/wp-content/uploads/2019/09/ESP8266-ESP-01-WiFi-Module-1.jpg

| Specification       | Value                               |
|---------------------|-------------------------------------|
| Processor           | ATmega328P 16 MHz                   |
| RAM                 | 2 KB                                |
| Wi-Fi               | Via separate ESP-01 module          |
| GPIO Pins           | 14 digital, 6 analog                |
| Combined Price      | Rs. 120 (Nano clone) + Rs. 120 (ESP-01) = Rs. 240 |

Best for: If you already own Arduino Nanos and want to reuse them. Also good if you want
to use them offline (no Wi-Fi needed) and log data locally to an SD card instead.

Trade-off: The ESP-01 communicates with the Nano via AT commands over UART, which is fiddly
to set up and uses two precious serial pins. RAM on the Nano is very limited. Not recommended
for new builds — a NodeMCU does everything better for a similar price.

---

#### Alternative 03 — Raspberry Pi Pico W

Photo reference:
- https://www.raspberrypi.com/products/raspberry-pi-pico/

| Specification       | Value                          |
|---------------------|--------------------------------|
| Processor           | RP2040 dual-core ARM Cortex-M0+|
| Clock Speed         | 133 MHz                        |
| RAM                 | 264 KB                         |
| Wi-Fi               | Built-in CYW43439              |
| GPIO Pins           | 26                             |
| Price               | Rs. 400-550                    |

Best for: Projects where you want to use MicroPython rather than C/Arduino, or where you
want a very stable and well-documented processor. The Pico W's Wi-Fi is newer and more
stable than the ESP8266's in crowded 2.4 GHz environments like malls and hospitals.

Trade-off: MicroPython is slower than compiled C for tight sensor timing loops. The Arduino
community and library support for parking/sensor projects is much larger for ESP8266/ESP32.

---

#### Alternative 04 — Arduino Uno (No Wi-Fi — Offline Only)

Photo reference:
- https://store.arduino.cc/cdn/shop/products/A000066_01.iso.jpg

| Specification       | Value               |
|---------------------|---------------------|
| Processor           | ATmega328P 16 MHz   |
| RAM                 | 2 KB                |
| Wi-Fi               | None                |
| GPIO Pins           | 14 digital, 6 analog|
| Price               | Rs. 150-500 (clone to genuine) |

Best for: Completely offline builds where you display slot status on a local LCD screen
at the entrance rather than a phone-based system. Good for very small lots (5-6 slots)
where simplicity matters more than remote access.

Trade-off: No Wi-Fi means no QR code system, no remote monitoring, and no phone-based
interface. This is a fundamentally different (simpler) system architecture.

---

## Component 02 — Parking Slot Sensor

### HC-SR04 Ultrasonic Distance Sensor

Photo references:
- Product image: https://robu.in/wp-content/uploads/2020/02/HC-SR04-Ultrasonic-Distance-Sensor-Module-1.jpg
- Wiring diagram: https://cdn.sparkfun.com/assets/b/3/0/b/a/51f1afb6ce395fb227000000.png

| Specification       | Value                    |
|---------------------|--------------------------|
| Operating Voltage   | 5V DC                    |
| Trigger Input       | 10 microsecond pulse      |
| Echo Output         | 5V (needs voltage divider for ESP8266) |
| Measuring Range     | 2 cm to 400 cm           |
| Accuracy            | +/- 3 mm                 |
| Beam Angle          | 15 degrees               |
| Pins Required       | 2 GPIO per sensor (TRIG + ECHO) |
| Current Draw        | 15 mA during measurement |
| Price               | Rs. 35-50                |

Why this was chosen: The HC-SR04 is the most widely used sensor in Indian maker projects.
Every electronics shop stocks it. The firmware, wiring, and troubleshooting information is
available in dozens of languages including Hindi. It detects anything that reflects sound —
cars, bikes, trucks, scooters — with no calibration, regardless of vehicle color or material.
At Rs. 40 per slot, it keeps the per-slot hardware cost under Rs. 100.

---

### Alternative Sensors

---

#### Alternative 01 — HY-SRF05 Ultrasonic Sensor (Recommended Upgrade)

Photo reference:
- https://robu.in/wp-content/uploads/2019/11/HY-SRF05-Ultrasonic-Distance-Sensor-1.jpg

| Specification       | Value                           |
|---------------------|---------------------------------|
| Operating Voltage   | 3.3V or 5V                      |
| Echo Output         | 3.3V safe (no voltage divider needed) |
| Range               | 2 cm to 450 cm                  |
| Accuracy            | +/- 2 mm                        |
| Extra Mode          | Single-wire mode available      |
| Price               | Rs. 60-80                       |

Best for: Any ESP8266 or ESP32 build where you want to skip the voltage divider resistors.
The SRF05 runs at 3.3V logic natively, so ECHO connects directly to GPIO without risk of
damaging the board. Rs. 20-30 more per slot but saves wiring complexity and reduces the
chance of accidental board damage.

Trade-off: Slightly less available in small local shops compared to HC-SR04. Order online.

---

#### Alternative 02 — VL53L0X Time-of-Flight Laser Sensor

Photo references:
- Product image: https://robu.in/wp-content/uploads/2019/11/VL53L0X-Time-of-Flight-Ranging-Sensor-1.jpg
- Breakout board: https://cdn-learn.adafruit.com/assets/assets/000/037/547/medium800/sensors_VL53L0X.jpg

| Specification       | Value                        |
|---------------------|------------------------------|
| Operating Voltage   | 3.3V                         |
| Interface           | I2C (only 2 wires for data)  |
| Range               | 3 cm to 200 cm               |
| Accuracy            | +/- 3%                       |
| Beam Width          | Very narrow — 25 degree cone |
| Weather             | Affected by bright sunlight  |
| Price               | Rs. 180-250                  |

Best for: Indoor parking lots, basement garages, and covered parking. The I2C interface
means you can daisy-chain up to 8 sensors using only 2 GPIO pins (SDA and SCL) with address
switching. This dramatically simplifies wiring in dense slot configurations.

Trade-off: Direct sunlight above 10,000 lux saturates the sensor and gives false readings.
Not suitable for open-air lots without a shade cover over the sensor. 4-5x more expensive
than HC-SR04.

---

#### Alternative 03 — IR Infrared Obstacle Sensor (Budget Option)

Photo reference:
- https://robu.in/wp-content/uploads/2019/11/IR-Obstacle-Avoidance-Sensor-Module-1.jpg

| Specification       | Value                          |
|---------------------|--------------------------------|
| Operating Voltage   | 3.3V to 5V                     |
| Output              | Digital HIGH/LOW only          |
| Detection Range     | 2 cm to 30 cm (adjustable pot) |
| Affected by Sunlight| Yes — significantly            |
| Pins Required       | 1 GPIO per sensor (signal only)|
| Price               | Rs. 20-30                      |

Best for: Indoor covered parking only, where the detection distance is short and
predictable. Cheapest option available. Good for a first prototype.

Trade-off: Sunlight and reflective surfaces cause constant false triggers outdoors. The
detection range is only 30 cm maximum, so mounting position is very sensitive. No distance
reading — only occupied/free, with no ability to adjust the threshold in firmware. Not
recommended for production outdoor installations.

---

#### Alternative 04 — Inductive Loop Sensor (Professional Grade)

| Specification       | Value                          |
|---------------------|--------------------------------|
| Detection Method    | Electromagnetic induction      |
| False Positives     | Near zero                      |
| Weather Resistance  | Complete — embedded in ground  |
| Works on            | Metal vehicles only            |
| Installation        | Requires cutting road surface  |
| Price               | Rs. 2,000-5,000 per slot       |

Best for: Permanent municipality or commercial installations where budget is not a concern
and reliability must be absolute over years of operation. Used in professional parking
management systems globally.

Trade-off: Cannot detect non-metallic vehicles (bicycles, certain scooters). Requires
civil work to cut loops into the floor — not possible in rented or temporary spaces.
Far higher cost per slot. Covered in this list only as a benchmark reference.

---

## Component 03 — GPIO Expander / Multiplexer

### CD74HC4067 16-Channel Multiplexer

Photo reference:
- https://robu.in/wp-content/uploads/2020/01/CD74HC4067-16-Channel-Multiplexer-Module-1.jpg

| Specification       | Value                           |
|---------------------|---------------------------------|
| Channels            | 16 input, 1 common output       |
| Control Pins        | 4 (S0-S3 select) + 1 enable     |
| Operating Voltage   | 2V to 6V                        |
| Signal Bandwidth    | Up to 1 MHz                     |
| Works with          | Digital and analog signals      |
| Price               | Rs. 40-60                       |

Why this was chosen: The ESP8266 only has enough usable GPIO pairs for 4-5 HC-SR04 sensors
directly. With one CD74HC4067, you can cycle through 16 sensors using only 5 pins total.
At Rs. 50, it is one of the cheapest ways to expand slot capacity per controller board.

---

### Alternative Multiplexers

---

#### Alternative 01 — 74HC4051 8-Channel Multiplexer

| Specification | Value                       |
|---------------|-----------------------------|
| Channels      | 8                           |
| Control Pins  | 3 (S0-S2) + 1 enable        |
| Price         | Rs. 20-35                   |

Best for: Zones with fewer than 8 slots where the full 16-channel version is overkill.
Saves one control pin. Same wiring logic, just half the channels.

---

#### Alternative 02 — PCF8574 I2C GPIO Expander

| Specification | Value                        |
|---------------|------------------------------|
| Interface     | I2C (2 wires only)           |
| Extra Pins    | 8 additional GPIO            |
| Chainable     | Yes — up to 8 on same bus    |
| Price         | Rs. 30-50                    |

Best for: If you are already using I2C sensors (like VL53L0X) and want to keep the bus
consistent. Can trigger HC-SR04 sensors through expanded GPIO with slight firmware changes.

---

## Component 04 — Server Hardware

### Raspberry Pi 4 Model B (2GB)

Photo reference:
- https://www.raspberrypi.com/products/raspberry-pi-4-model-b/

| Specification       | Value                         |
|---------------------|-------------------------------|
| Processor           | Quad-core ARM Cortex-A72      |
| RAM                 | 2 GB LPDDR4                   |
| Network             | Gigabit Ethernet + Wi-Fi      |
| USB                 | 2x USB 3.0, 2x USB 2.0        |
| Power               | 5V 3A USB-C                   |
| OS Support          | Raspberry Pi OS (Linux)       |
| Price               | Rs. 3,500-4,500               |

Why this was chosen: Runs full Linux, Node.js, and the backend server continuously on
5W of power. Silent, compact, no moving parts. Can be mounted inside the parking lot's
electrical cabinet. Serves dozens of simultaneous phone connections without performance
issues.

---

### Alternative Server Hardware

---

#### Alternative 01 — Any Old Laptop or Desktop (Free)

Best for: Getting the system running immediately without buying anything new. A 10-year-old
laptop running Ubuntu works perfectly. Node.js on an i3 CPU can serve hundreds of clients
simultaneously. This is the recommended starting point — validate the system works before
buying dedicated hardware.

Trade-off: A laptop left running 24/7 draws 20-40W vs 5W for a Pi. Not ideal for permanent
low-cost operation. But for a trial or a small lot, it is perfectly fine.

---

#### Alternative 02 — Orange Pi Zero 2 (Budget Pi Alternative)

| Specification | Value                      |
|---------------|----------------------------|
| Processor     | Allwinner H616 quad-core   |
| RAM           | 1 GB                       |
| Network       | Ethernet + Wi-Fi           |
| Power         | 5V 2A                      |
| Price         | Rs. 1,800-2,500            |

Best for: Reducing cost when Raspberry Pi 4 units are unavailable or overpriced. Runs
Armbian Linux and Node.js without issues.

Trade-off: Community support is smaller than Raspberry Pi. Some initial setup steps
differ. Not as beginner-friendly.

---

#### Alternative 03 — Cloud VPS (Remote Deployment)

Best for: Installations where the parking lot does not have a reliable local machine,
or where you want to monitor multiple parking lots from one central server. DigitalOcean
and AWS Lightsail offer servers starting at $4-6 per month.

Trade-off: Requires internet connectivity at the parking lot at all times. If the internet
goes down, users cannot check availability. Not recommended as the primary option for
Indian installations where internet reliability varies.

---

## Component 05 — Power Supply

### 5V 2A USB Adapter

Photo reference:
- Standard USB wall adapter, widely available

| Specification | Value               |
|---------------|---------------------|
| Output        | 5V 2A (10W)         |
| Connector     | USB-A or Micro USB  |
| Price         | Rs. 60-120          |

Why this was chosen: Powers the ESP8266 and up to 8 HC-SR04 sensors comfortably.
Available at every mobile accessories shop in India.

---

### Alternative Power Sources

---

#### Alternative 01 — Solar Panel + Charge Controller + Li-ion Battery (Off-Grid)

| Component           | Specification     | Price        |
|---------------------|-------------------|--------------|
| Solar Panel         | 10W 12V monocrystalline | Rs. 700-900 |
| Solar Charge Controller | PWM 10A 12V/5V out | Rs. 300-500 |
| Li-ion Battery Pack | 3.7V 5000mAh 18650 cells | Rs. 600-900 |
| Total               |                   | Rs. 1,600-2,300 |

Best for: Open-air parking lots, event grounds, mandis, rural markets, and any location
where extending a 230V grid power line to the sensor zone would cost more than Rs. 2,000.
A 10W panel can fully charge the battery in 4-6 hours of direct sunlight and keep the
ESP8266 running through a 12-hour night.

Trade-off: Higher upfront cost per zone. Requires periodic cleaning of the solar panel
(once a month). Battery degrades over 3-4 years and needs replacement.

---

#### Alternative 02 — Power over Ethernet (PoE)

| Specification | Value                           |
|---------------|---------------------------------|
| Standard      | IEEE 802.3af (PoE)              |
| Power         | 15W over CAT5/6 cable           |
| Required      | PoE switch + PoE splitter       |
| Price         | Rs. 1,500-3,000 for PoE switch + Rs. 200 per splitter |

Best for: Large covered parking structures (malls, hospitals, airports) where CAT6
Ethernet is already run to each floor. Power and data over one cable simplifies installation
significantly. Most reliable power delivery option — no battery, no solar variability.

Trade-off: Higher infrastructure cost. Only practical if Ethernet is already in place.

---

## Component 06 — Optional Add-ons

### WS2812B Addressable LED Strip

Photo reference:
- https://robu.in/wp-content/uploads/2020/03/WS2812B-5050-RGB-LED-Strip-1.jpg

| Specification | Value                           |
|---------------|---------------------------------|
| Control Pins  | 1 data pin per strip            |
| LEDs per Meter| 30 or 60                        |
| Colors        | Full RGB, individually addressed |
| Voltage       | 5V                              |
| Current       | 60mA per LED at full white      |
| Price         | Rs. 200-350 per meter           |

Use case: One LED per slot visible from the aisle. Green = free, red = occupied.
Drivers can see available slots without looking at a phone. Each strip segment is
driven directly from the ESP8266 using the FastLED or Adafruit NeoPixel library.

---

### Alternative LED Options

#### WS2811 Strip (Cheaper, 12V)
Rs. 150-250 per meter. Runs on 12V so less voltage drop over long runs. Requires a
12V power supply in addition to the 5V supply for the board.

#### Individual Red/Green LED + Resistor (Absolute Minimum Cost)
Rs. 2-5 per slot. One green and one red 5mm LED per slot, driven through a transistor
from GPIO. No addressable data protocol needed. Suitable if you only need a simple
occupied/free indicator and do not need color transitions or effects.

---

## Decision Guide — Which Components to Choose

| Situation                            | Board         | Sensor        | Power         | Est. Per-Slot Cost |
|--------------------------------------|---------------|---------------|---------------|-------------------|
| Indoor, tight budget                 | ESP8266 NodeMCU | HC-SR04     | USB adapter   | Rs. 60-80         |
| Indoor, easier wiring                | ESP32 DevKit  | VL53L0X (I2C)| USB adapter   | Rs. 300-350       |
| Outdoor, covered (canopy/shade)      | ESP8266 NodeMCU | HC-SR04     | USB adapter   | Rs. 60-80         |
| Outdoor, open sun                    | ESP8266 NodeMCU | HC-SR04     | Solar kit     | Rs. 180-220       |
| Outdoor, sun, want more accuracy     | ESP32 DevKit  | SRF05         | Solar kit     | Rs. 240-280       |
| Large installation (50+ slots)       | ESP32 DevKit  | HC-SR04 + 4067 mux | PoE      | Rs. 150-200       |
| Maximum reliability, budget secondary| ESP32 DevKit  | SRF05         | PoE           | Rs. 400-500       |
| First-time builder, want it simple   | ESP8266 NodeMCU | HC-SR04     | USB adapter   | Rs. 60-80         |

---

## Where to Buy in India

All components listed here are available from these suppliers. Prices vary — compare before ordering.

Robu.in is the most reliable for sensors, ESP boards, and multiplexers with good shipping
across India and reasonable prices. Most components arrive in 3-5 days.

Amazon India stocks almost everything but prices are higher for electronic components.
Useful when you need guaranteed next-day delivery.

SP Road, Bangalore and Lamington Road, Mumbai are the best places to buy in bulk at the
lowest prices if you are setting up a large installation. Walk-in customers can negotiate
on orders of 20+ units.

Quartzcomponents.com is good for resistors, terminal blocks, and passive components in bulk.

For solar components specifically, Loom Solar and Luminous have distributor networks
across Tier 2 cities and are a reliable source for panels and charge controllers.

---

Contact: carmodbhai@gmail.com
