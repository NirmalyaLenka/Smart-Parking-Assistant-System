# Bill of Materials — Smart Parking Assistant

All prices are approximate Indian Rupee values as of 2024.
Prices may vary by supplier and city. Check Robu.in, Amazon India, and local markets.

## Per-Slot Components (replicate for each parking slot)

| Component          | Quantity | Unit Price (INR) | Total Per Slot | Notes                                      |
|--------------------|----------|------------------|----------------|--------------------------------------------|
| HC-SR04 Sensor     | 1        | 40               | 40             | Widely available, reliable at 2-400cm range|
| Voltage divider resistors (1k + 2k) | 1 set | 2 | 2        | Protects ESP8266 from 5V ECHO signal       |
| 4-core cable (per meter) | 2   | 8                | 16             | Alarm cable or CAT5                        |
| Cable clips/ties   | 4        | 1                | 4              | For routing cables cleanly                 |
| Per-slot total     |          |                  | ~62            |                                            |

## Per-Zone Controller (one ESP8266 board covers up to 8 slots)

| Component              | Quantity | Unit Price (INR) | Notes                                       |
|------------------------|----------|------------------|---------------------------------------------|
| ESP8266 NodeMCU v3     | 1        | 220              | CP2102 version recommended                  |
| CD74HC4067 Multiplexer | 1        | 45               | Only needed if managing more than 4 sensors |
| Small project box (ABS)| 1        | 60               | For housing the board and connections       |
| 5V 2A USB adapter      | 1        | 80               | Power supply for ESP8266 and sensors        |
| USB cable micro        | 1        | 30               | Power and programming                       |
| Terminal block strip   | 1        | 25               | For clean sensor wire connections           |
| Per-zone total         |          | ~460             |                                             |

## System-Level (one per installation)

| Component                    | Quantity | Unit Price (INR) | Notes                                           |
|------------------------------|----------|------------------|-------------------------------------------------|
| Raspberry Pi 4 (2GB) or old laptop | 1  | 3500 / 0         | Runs backend server. Any Linux machine works.   |
| Wi-Fi router                 | 1        | 800              | Creates local network for ESP8266 boards        |
| QR code printout (laminated) | 2-3      | 20               | Entrance gate signs                             |
| System total                 |          | ~4300            |                                                 |

## Total Cost Estimate by Lot Size

| Lot Size (slots)  | Slot Hardware | Zones Needed | Zone Hardware | System Hardware | Grand Total (INR) |
|-------------------|---------------|--------------|---------------|-----------------|--------------------|
| 10 slots          | 620           | 2            | 920           | 4300            | ~5840              |
| 20 slots          | 1240          | 3            | 1380          | 4300            | ~6920              |
| 40 slots          | 2480          | 5            | 2300          | 4300            | ~9080              |
| 100 slots         | 6200          | 13           | 5980          | 4300            | ~16480             |

Compare this to commercial ANPR-based systems which typically start at Rs. 1,50,000 and go up to Rs. 5,00,000
for 20-40 slots, not including annual licensing and maintenance contracts.

## Where to Buy

Robu.in — Best selection for ESP8266, sensors, and modules with fast shipping
Amazon India — Good for cables, adapters, project boxes
Local electronics market (Lamington Road Mumbai / SP Road Bangalore / Chandni Chowk Delhi) — Cheapest
Quartzcomponents.com — Good for bulk resistors and connectors
