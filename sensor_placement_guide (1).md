# Sensor Placement Guide

## Recommended Mounting Heights

For overhead mounting (sensor faces down at the parking bay floor):
- Mount 1.5 to 2.5 meters above floor level
- Position the sensor directly above the center of the slot
- Angle must be perpendicular to the floor (straight down, 0 degrees tilt)
- At this height, a car roof is typically 0.5-1.0m below the sensor, giving a clear occupied reading

For side-entry mounting (sensor faces horizontally across the slot entrance):
- Mount at bumper height: 30 to 50 cm above the floor
- Position at the far end of the slot, facing toward the entrance
- Distance to slot entrance should be 3 to 5 meters
- Trigger threshold set to 80-90% of that distance

## Environment Considerations

Outdoor installations:
- Use a weatherproof enclosure (IP65 or better). A small ABS project box with rubber gasket works.
- Keep the sensor faces clear of direct water spray. A small overhanging lip of 2-3 cm helps.
- Avoid mounting where early morning or late afternoon sun shines directly at the sensor face.
  Ultrasonic sensors are not affected by light, but temperature gradients on a hot surface can cause spurious reflections.
- In very cold climates, condensation on the sensor face can cause inaccurate readings.
  A small heating resistor inside the enclosure solves this but adds complexity.

Indoor installations:
- No special precautions needed beyond keeping sensor faces clean.
- Fluorescent light fixtures and metal surfaces create multipath reflections but are consistent enough not to trigger false readings at the thresholds used.

## Common Problems and Solutions

Problem: Sensor reads vehicle as free despite one being parked.
Cause: Vehicle roof is highly absorptive (open-top vehicle, angled roof, large sunroof).
Solution: Lower the mounting height slightly or use two sensors per slot at different angles.

Problem: Sensor triggers occupied when slot is empty.
Cause: Threshold set too high. A sensor mounted at 2m reads an empty floor at roughly 2m.
  If threshold is 2.5m, it will never read as free.
Solution: Set the threshold to about 60-70% of mounting height. At 2m height, use 120-140cm threshold.

Problem: Sensor gives random readings in rain.
Cause: Heavy rain creates false reflections mid-air.
Solution: Increase the OCCUPIED_THRESHOLD_CM to a lower value (closer distance only).
  Rain drops are too light to reflect at the power levels HC-SR04 uses but heavy continuous rain can cause issues.
  Mount sensors under an awning where possible.

Problem: Two adjacent sensors interfere with each other.
Cause: The ultrasonic pulse from sensor A is picked up by the ECHO pin of sensor B.
Solution: Trigger sensors sequentially with a 50ms gap, never simultaneously.
  The firmware already does this in the polling loop.

## Cable Routing

Run sensor cables in conduit where possible for protection and a clean appearance.
Keep sensor cables away from mains power cables to avoid interference.
Use cable ties every 30 cm along exposed runs.
Label each cable at both ends with the slot ID using a cable marker or adhesive label.
At the ESP8266 end, use a terminal block strip rather than soldering directly to the board,
so individual sensors can be replaced without re-soldering.

## Testing Before Permanent Mounting

Always bench-test each sensor before mounting:
1. Hold the sensor at the planned mounting height above a flat surface.
2. Check Serial Monitor for distance readings.
3. Place a piece of cardboard at car-roof height and confirm reading changes correctly.
4. Then mount permanently and test again after mounting.

A five-minute bench test saves considerable time compared to re-mounting a sensor that turns out to be faulty.
