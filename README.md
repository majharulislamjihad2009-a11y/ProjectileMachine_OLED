# ProjectileMachine_OLED

ProjectileMachine_OLED is a physics-accurate projectile motion simulator built on an ESP8266 NodeMCU with a 0.96" SSD1306 OLED display (128×64).

This project demonstrates how real-time physics simulation can run smoothly on constrained embedded hardware using a minimal and clean user interface.

All motion is calculated live.
There are no preset animations or lookup tables.
Every run depends strictly on user-selected initial height, angle, velocity, and gravity.

---

## Demo

YouTube video:
https://youtube.com/shorts/ClX_AxegJIg?si=9OpkDlyONRSiR_SK

---

## Features

- Real-time projectile motion simulation
- No pre-rendered trajectories
- Minimal, non-crowded OLED UI
- Initial height selection with skip option
- Gravity modes:
  - Earth (9.81 m/s²)
  - Moon (1.62 m/s²)
  - Custom gravity via Morse input
- Morse-based numeric input with decimal support (.-)
- Live trajectory prediction before launch
- Real-time velocity vector components (Vx, Vy)
- Camera-follow scrolling ground
- Subtle projectile motion trail
- Optional buzzer feedback

---

## Physics Model

Standard projectile equations with initial height:

x(t) = v0 · cos(θ) · t  
y(t) = h0 + v0 · sin(θ) · t − ½ · g · t²

Implementation details:

- Fixed time-step integration
- Gravity affects only vertical velocity
- Accurate ground-impact interpolation
- No lookup tables
- No hardcoded paths

All calculations are computed in real time on the ESP8266.

---

## User Flow

### 1. Power On
- Startup animation
- Automatically enters height selection

### 2. Initial Height Selection
Set initial height h0 in meters.

Controls:
- UP (hold): increase
- DOWN (hold): decrease
- ENTER: confirm
- ENTER immediately: skip (0m)

Only numeric value is shown for clarity.

### 3. Gravity Selection
Options:
- Earth
- Moon
- Custom

Controls:
- UP / DOWN: change
- ENTER: confirm

If Custom is selected, Morse input opens.

### 4. Custom Gravity (Morse Input)

Rules:
- Dot: .
- Dash: -
- Decimal point: .-

Buttons:
- UP: dot
- DOWN: dash
- ENTER: confirm
- Long ENTER: backspace/clear

Invalid input triggers error sound (optional).

### 5. Angle Mode
- Hold UP/DOWN to rotate cannon
- Live trajectory preview
- ENTER switches to velocity

### 6. Velocity Mode
- Hold UP/DOWN to adjust speed
- Live trajectory preview
- ENTER launches

### 7. During Simulation
- Real-time physics
- Vx and Vy vectors displayed
- Camera follows projectile
- Ground scroll effect
- Motion trail
- Clean stop on landing

### 8. Result Screen
Shows:
- R: range
- H: max height
- T: flight time

---

## Hardware Requirements

- ESP8266 NodeMCU (ESP-12E/ESP-12F)
- SSD1306 OLED 128×64 (I2C)
- 3 push buttons
- Optional active buzzer
- Breadboard + jumper wires
- USB cable

---

## Wiring

### Buttons and Buzzer (INPUT_PULLUP)

Buttons connect GPIO → GND (pressed = LOW).

| Control | GPIO | NodeMCU Pin |
|--------|------|-------------|
| UP     | 14   | D5 |
| DOWN   | 12   | D6 |
| ENTER  | 13   | D7 |
| BUZZER | 15   | D8 |

### OLED (I2C)

| OLED | NodeMCU |
|-------|-----------|
| VCC | 3V3 |
| GND | GND |
| SDA | D2 (GPIO4) |
| SCL | D1 (GPIO5) |

---

## Arduino IDE Setup

### Board Support URL
Add to:
File → Preferences → Additional Boards Manager URLs

http://arduino.esp8266.com/stable/package_esp8266com_index.json

Install:
ESP8266 by ESP8266 Community

---

## Required Libraries

Install from Library Manager:

- Adafruit GFX
- Adafruit SSD1306

---

## Recommended Settings

- Board: NodeMCU 1.0 (ESP-12E)
- CPU: 80 MHz
- Flash: 4MB
- Upload Speed: 115200
- Erase Flash: Only Sketch

---

## Flashing

Open:

src/ProjectileMachine_OLED/ProjectileMachine_OLED.ino

Select COM port and upload.

---

## Repository Structure

src/ProjectileMachine_OLED/     → main sketch  
lib/                           → libraries  
docs/                          → diagrams/screenshots  
README.md                      → documentation  

---

## Troubleshooting

OLED not detected:
- Check SDA/SCL wiring
- Verify I2C address

Buttons not responding:
- Ensure INPUT_PULLUP wiring (GPIO → GND)

Simulation unstable:
- Use recommended board settings
- Avoid blocking delays

---

## License

MIT License

---

## Author

Majharul Islam Jihad

This project was built as a long-term learning experiment in:

- embedded systems
- physics simulation
- real-time rendering
- optimization under memory constraints

AI was used only as a technical aid for:
- equation validation
- debugging suggestions
- documentation improvements

All architecture, implementation, and testing were done manually.

The goal was understanding and engineering accuracy, not automation.

