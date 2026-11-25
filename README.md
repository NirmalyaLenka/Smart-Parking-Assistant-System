# Smart Parking Assistant System

> **Prototype status:** The repository contains a browser simulation and early ESP8266/Node.js components, but the current filenames, server paths, and six-sensor pin map are inconsistent. It is not a ready-to-deploy live parking system as checked in.

## What is included

The intended data path is ESP8266 ultrasonic readings -> `POST /api/update` -> in-memory Node.js state -> `GET /api/status`. The root `index.html` is a standalone interactive simulation; it does not poll the API and should not be presented as live sensor data.

| Tracked root file | Purpose |
|---|---|
| `parking_sensor.ino` | ESP8266 sketch. Reads configured HC-SR04 sensors and posts JSON to the backend. |
| `config (2).h` | Example Wi-Fi/server/zone settings. The sketch includes `config.h`, so this suffixed filename does not satisfy that include as-is. It currently contains placeholders. |
| `server (1).js` | Express API server. Its static-file paths expect a `backend/` folder next to a `frontend/` folder, which are not present in the tracked root tree. |
| `package (1).json` | Node package manifest content. npm does not automatically recognize this suffixed filename as `package.json`. |
| `env (1).example` | Example `PORT` and `NODE_ENV` settings; the filename is not the conventional `.env.example`. |
| `index.html` | Standalone simulated slot-map demo with manual/random scenarios; it is not connected to `server (1).js`. |
| `generate_qr.py` | Generates two labeled QR-code PNGs from the `PARKING_URL` constant. Requires `qrcode[pil]`. |
| `bill_of_materials (1).md` | BOM and approximate prices, explicitly dated 2024. Recheck current prices before purchasing. |
| `components_and_alternatives.md` | Component reference and alternatives. |
| `sensor_placement_guide (1).md` | Sensor mounting and environment notes. |
| `wiring_diagram (2).md` | HC-SR04/ESP8266 wiring notes; see the blocking pin-conflict warning below. |
| `gitignore (5)` | Ignore-pattern text only. Git does not treat this suffixed filename as an active `.gitignore`. |
| `LICENSE (4)` | License-named file with a suffix; confirm licensing rather than assuming GitHub recognizes it as the repository's standard license file. |

## Architecture and API

- `POST /api/update` accepts a JSON object containing `zone`, `vehicleType`, `timestamp`, and a `slots` array with `id` and `occupied` fields.
- `GET /api/status` returns current in-memory slot state and summary counts.
- The server initializes default slots in memory. State is not persisted across server restarts.
- The firmware defaults to a two-second posting interval. A sensor timeout currently sets that slot to `false` (free) in the outgoing reading, so a failed sensor can be misrepresented as an empty slot.

## Important blockers before hardware use

### Six-sensor pin conflicts

The current `parking_sensor.ino` arrays reuse GPIOs: `D2` is both a trigger and echo pin, and `D6` / `D8` are repeated as echo inputs. The wiring document mentions a multiplexer, but the sketch contains no multiplexer driver. Do **not** wire or deploy a six-sensor installation using the current mapping. A corrected pin plan and corresponding firmware change are required before hardware use; this documentation-only pass leaves firmware pins unchanged.

HC-SR04 ECHO is commonly 5 V while ESP8266 GPIO is 3.3 V tolerant. Use a correctly sized voltage divider or level shifter on every ECHO line; do not connect a 5 V ECHO output directly to an ESP8266 GPIO.

### Backend and filename mismatch

As committed, `npm start` will not find the script named in the package manifest because the files are `package (1).json` and `server (1).js`. The server also serves `../frontend` and `../frontend/index.html`, but no `frontend/` directory exists. The firmware includes `config.h`, while the tracked header is named `config (2).h`. A proper root `.gitignore` was added in this maintenance batch; the legacy `gitignore (5)` file remains inert. Ignore rules do not protect files that are already tracked, so never commit real credentials.

## Local preview

Open the root `index.html` in a browser to try the simulated slot map. The Random/manual scenarios are generated in the page; this is not a live reading from an ESP8266 or the backend.

## Preparing a local backend copy

To match the paths expected by the server without changing the tracked repository, make a local working copy with this layout:

```text
backend/
  server.js       # copy of root server (1).js
  package.json    # copy of root package (1).json
  .env.example    # copy of root env (1).example
frontend/
  index.html      # copy of root index.html (simulation only)
```

Then, from `backend/`, run `npm install` and `npm start`. This can prepare the API server path after copying the files, but the copied HTML remains a simulation and is not wired to `/api/status`. No successful server run is claimed here.

## Preparing a local firmware sketch

Create a local `firmware/parking_sensor/` sketch folder, copy `parking_sensor.ino` into it as `parking_sensor.ino`, and copy the contents of `config (2).h` into `firmware/parking_sensor/config.h`. Replace the Wi-Fi placeholders and set `SERVER_URL` to the backend machine's LAN address and port. Install ArduinoJson 6.x and the ESP8266 board package.

**Do not flash the six-sensor sketch for real use until the pin collisions are resolved in firmware and the wiring is revalidated.** Keep the backend on a trusted local network: the current API has permissive CORS and no authentication, and the state is volatile.

## QR code generation

Install the Python dependency with `python -m pip install "qrcode[pil]"`, edit `PARKING_URL` in `generate_qr.py` to the intended reachable URL, and run `python generate_qr.py`. The script writes `qr_four_wheeler.png` and `qr_two_wheeler.png` beside the script. The generated QR codes do not make the root simulation a live dashboard.

## Verification status

No end-to-end sensor/API test, firmware build, hardware test, or security test was performed for this documentation update. Component prices in the BOM are historical estimates, not current quotes.

## Maintenance & Audit

- **Audit date:** 18 November 2026
- **Documentation status:** Repository paths, firmware/config names, server expectations, dependencies, BOM, and wiring conflicts reviewed.
- **Verification status:** Source/document inspection only; live integration and hardware behavior remain unverified.
- **Scope:** Firmware, pin assignments, dependencies, and hardware behavior were not changed.

## Contact

Maintained up to 25 November 2026.

If you encounter any problems, please contact [carmodbhai@gmail.com](mailto:carmodbhai@gmail.com).
