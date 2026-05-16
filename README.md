# Smart Parking Assistant System

A low-cost IoT-based smart parking system that helps drivers find available parking spots in real time by scanning a QR code at the entrance. The system supports both two-wheelers and four-wheelers in separate zones, and updates slot availability automatically using ultrasonic sensors connected to an ESP8266 microcontroller.

---

## What This Project Does

When a driver arrives at the parking lot, they scan a QR code placed at the entrance gate. This opens a webpage on their phone that shows exactly which slots are free and which are occupied, split by vehicle type. The page refreshes automatically so the information is always live. No app download, no login, no complicated setup on the user side.

On the hardware side, each parking slot has an ultrasonic sensor mounted above it or at the entry of each bay. These sensors measure distance and detect whether a vehicle is present. The readings go to an ESP8266 NodeMCU board over Wi-Fi, which pushes the data to a lightweight backend server. The frontend reads from that server and renders the slot map.

---

## Why This Approach

Most commercial parking management systems cost lakhs of rupees for hardware alone, require proprietary software, and take months to install. This system uses components that cost under Rs. 300 per slot, runs on open-source software, and can be set up in a weekend. For small colleges, hospitals, residential societies, market complexes, and small businesses, this is a practical and immediate solution.

The QR code approach means zero friction for users. They do not need to install anything. Any phone with a camera works.

---

## Repository Structure

Every file in this repository has a specific purpose. Here is what each one does and why it exists.

### Root Level

- `README.md` - This file. Explains the full project from hardware to software.
- `LICENSE` - MIT license. You are free to use, modify, and deploy this for any purpose.
- `.gitignore` - Tells Git to ignore sensitive config files, node_modules, and compiled binaries so they do not end up in version control.

### /firmware

This folder contains the code that runs directly on the ESP8266 NodeMCU board.

- `firmware/parking_sensor.ino` - The main Arduino sketch. It reads all the ultrasonic sensors, decides if a slot is occupied or free, and sends a JSON payload to the backend server over Wi-Fi using HTTP POST. It runs in a loop and posts every two seconds.
- `firmware/config.h` - Stores your Wi-Fi credentials (SSID and password) and the backend server IP address. This file is listed in .gitignore so you never accidentally commit your passwords to GitHub.
- `firmware/wiring_diagram.md` - Plain text description of how to connect each sensor to the ESP8266 GPIO pins, including voltage levels and ground connections. Read this before you solder anything.

### /backend

This folder has a simple Node.js server that acts as the bridge between the ESP8266 and the user-facing frontend.

- `backend/server.js` - The core server. It runs on Express and exposes two endpoints: one POST endpoint that receives sensor data from the ESP8266, and one GET endpoint that the frontend polls to fetch the current slot status. It also serves the frontend HTML file when accessed from a browser.
- `backend/package.json` - Lists the Node.js dependencies (just Express and cors). Run `npm install` in this folder to get started.
- `backend/.env.example` - Shows which environment variables the server expects, like the port number. Copy this to `.env` and fill it in. The actual `.env` file is not committed.

### /frontend

This is what the user sees when they scan the QR code.

- `frontend/index.html` - The complete single-page parking display. Shows a visual grid of all slots, color-coded green for free and red for occupied. Separate sections for two-wheelers and four-wheelers. Auto-refreshes every three seconds. Works on any screen size.
- `frontend/style.css` - All the visual styling. Uses a dark industrial theme with clear color coding. Written in plain CSS so there is no build step required.
- `frontend/script.js` - Fetches the slot data from the backend every three seconds and updates the slot grid without reloading the page. Handles connection errors gracefully so the display does not break if the server hiccups.

### /hardware

Documentation for the physical build.

- `hardware/bill_of_materials.md` - Complete component list with approximate prices in Indian Rupees, quantity needed per slot, and where to source them (Amazon India, Robu.in, local electronics shops). Total system cost for a 20-slot parking lot is estimated here.
- `hardware/sensor_placement_guide.md` - Explains how far above the ground to mount the sensors, angle requirements, and how weather and sunlight can affect readings. Also covers enclosure suggestions for outdoor use.
- `hardware/pcb_notes.md` - Notes for anyone who wants to make a simple custom PCB to hold the ESP8266 and sensor connectors neatly instead of using a breadboard permanently.

### /qr-generator

- `qr-generator/generate_qr.py` - A small Python script that takes your deployed frontend URL and generates a QR code image ready to print. Install qrcode library with pip, update the URL variable, and run it. The output is a high-resolution PNG you can get printed at any print shop.

### /demo

- `demo/index.html` - A fully self-contained demonstration page that runs in any browser without any server or hardware. It simulates the sensor data with random slot updates so you can see the full system in action before buying a single component. This is also the file to share when pitching the project to someone.

---

## Hardware Components

The system is built around affordable, widely available parts.

The ESP8266 NodeMCU is the main controller. It costs around Rs. 200-250 and has built-in Wi-Fi, which is the core reason it was chosen over Arduino Uno or ATmega boards. No separate Wi-Fi shield needed.

HC-SR04 ultrasonic sensors do the detection. They cost Rs. 35-50 each, are extremely reliable for detecting vehicles at distances between 5 cm and 3 meters, and are available everywhere in India. Each sensor covers one parking slot.

Power is supplied through a standard 5V adapter or a PoE setup if Ethernet is already available at the site. The ESP8266 with 20 sensors draws well under 500mA total.

A single ESP8266 can handle up to 10-12 sensors if you use a multiplexer like the CD74HC4067. For larger lots, multiple ESP8266 boards can be deployed, each covering a zone, and all reporting to the same backend.

---

## Getting Started

### Step 1 - Set Up the Backend

You need Node.js installed on a computer or a small server like Raspberry Pi.

```
cd backend
npm install
cp .env.example .env
# Edit .env and set your port
node server.js
```

Note the IP address of this machine. Your ESP8266 and your QR code both need to point to it.

### Step 2 - Flash the Firmware

Open the firmware folder in Arduino IDE. Install the ESP8266 board package if you have not already. Open `config.h` and fill in your Wi-Fi name, password, and the backend server IP. Upload `parking_sensor.ino` to your NodeMCU.

### Step 3 - Wire the Sensors

Follow the wiring guide in `hardware/wiring_diagram.md`. Each HC-SR04 uses two GPIO pins (Trig and Echo). Test with the Serial Monitor first to confirm readings before mounting.

### Step 4 - Generate and Print the QR Code

Update the URL in `qr-generator/generate_qr.py` to point to your backend server address and run it. Print the generated QR code and place it at the parking entrance, ideally laminated or behind a plastic cover.

### Step 5 - Test

Open `demo/index.html` in a browser to verify the frontend looks correct. Then switch the frontend to point at your live backend and scan the QR code from a phone.

---

## Future Scope

This system was designed to be extended. Here are the directions it can naturally grow in.

Automatic barrier integration is the most immediate next step. A servo motor or relay connected to the ESP8266 can control a boom barrier. When all four-wheeler slots are full, the barrier stays closed and the entry display shows a full message, preventing drivers from entering a congested lot.

License plate recognition can be layered on using a Raspberry Pi camera at the entrance. This enables entry logging without any manual registration.

Monthly or daily reports can be added to the backend to show peak hours, average occupancy, and slot utilization rates. Useful for parking lot owners to plan staffing and pricing.

Payment integration through UPI QR codes can be embedded in the slot detail page so drivers can pay for their parking time without visiting a payment counter.

Multi-floor parking management is straightforward. Each floor gets its own set of ESP8266 nodes, all reporting to the same backend. The frontend shows a floor selector.

Solar power with a small panel and battery bank makes this completely off-grid, suitable for open grounds, event venues, and rural sites.

Smart city integration is the long-term application. Municipal corporations can deploy this across hundreds of street-side and public lots and feed the data into a city-wide parking app. This reduces traffic from people circling looking for parking, which is a meaningful fuel and emissions reduction.

Hospital and airport applications benefit enormously from the accessibility filtering feature, where handicapped-accessible slots are tracked separately and reserved appropriately.

---

## Contact

If you are deploying this system, have questions, or want to collaborate on extending it, reach out at carmodbhai@gmail.com

Bug reports and pull requests are welcome. If you build this and it works, sharing a photo or description of your setup is appreciated.

---

## License

MIT License. Use it freely.
