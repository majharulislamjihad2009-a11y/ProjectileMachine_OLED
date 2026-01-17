# ProjectileMachine_OLED

ProjectileMachine_OLED is a physics-accurate projectile motion simulator built on an ESP8266 NodeMCU with a 0.96" SSD1306 OLED display.  
The project is designed to demonstrate real-time physics simulation on constrained embedded hardware using a minimal, clean user interface.

The simulation is calculated live. There are no preset paths or fake animations.  
Every run depends strictly on user-selected initial height, angle, velocity, and gravity.

---

## Main Features

- Real-time projectile motion simulation
- Minimal, non-crowded OLED UI (128×64)
- Initial height selection with skip option
- Gravity modes:
  - Earth (9.81 m/s²)
  - Moon (1.62 m/s²)
  - Custom gravity via Morse input
- Morse-based numeric input with decimal support (`.-`)
- Live trajectory prediction before launch
- Real-time velocity vector components (Vx and Vy)
- Camera-follow effect with scrolling ground
- Subtle motion trail behind projectile
- Exactly two realistic bounces with energy loss
- Optional buzzer feedback for input and errors

---

## Physics Model (Overview)

The simulation follows standard projectile motion equations with initial height:

x(t) = v0 · cos(θ) · t
y(t) = h0 + v0 · sin(θ) · t − ½ · g · t²


Implementation details:
- Fixed time-step integration
- Gravity affects only vertical velocity
- Accurate ground-impact interpolation (prevents overshoot)
- Energy loss applied on bounces
- No lookup tables or hardcoded trajectories

---

## How to Use (User Flow)

### 1. Power On
- Device boots and shows a short startup animation
- Automatically proceeds to the height selection screen

---

### 2. Initial Height Selection
Purpose: Set initial height `h0` in meters

Controls:
- UP (hold): increase height
- DOWN (hold): decrease height
- ENTER: confirm and continue
- ENTER immediately: skip (height defaults to 0)

Only the numeric value is shown to keep the display clean.

---

### 3. Gravity Selection
Options:
- Earth (default)
- Moon
- Custom

Controls:
- UP / DOWN: change selection
- ENTER: confirm

If Custom is selected, the Morse input screen opens.

---

### 4. Custom Gravity Input (Morse)

Gravity can be entered as a decimal value (example: 9.81).

Morse rules:
- Dot: `.`
- Dash: `-`
- Decimal point: `.-`

Button mapping:
- UP: dot
- DOWN: dash
- ENTER: confirm symbol
- Long-press ENTER: backspace / clear

Invalid input triggers an error sound (if buzzer enabled).

---

### 5. Simulation Screen

#### Angle Adjustment Mode
- Hold UP / DOWN to rotate the cannon
- Live predicted trajectory updates in real time
- ENTER switches to velocity adjustment

#### Velocity Adjustment Mode
- Hold UP / DOWN to change launch velocity
- Live predicted trajectory updates
- ENTER starts the simulation

---

### 6. During Simulation
- Projectile follows real-time physics
- Velocity component arrows show:
  - Horizontal velocity (Vx)
  - Vertical velocity (Vy), changing with time
- Projectile stays near the screen center (camera-follow effect)
- Ground scrolls backward to simulate motion
- Motion trail shows recent positions
- On landing:
  - Exactly two bounces occur
  - Energy loss is applied
  - Simulation stops cleanly

---

### 7. Result Display
After completion, a minimal summary is shown:
- R: horizontal range
- H: maximum height
- T: total flight time

---

## Hardware Requirements

- ESP8266 NodeMCU (ESP-12E / ESP-12F)
- SSD1306 OLED 128×64 (I2C)
- 3 push buttons
- Optional active buzzer
- Breadboard and jumper wires
- USB cable for flashing

---

## Wiring

### Button Wiring (INPUT_PULLUP)

Buttons connect between GPIO and GND.  
Pressed state = LOW.

| Function | GPIO | NodeMCU Pin |
|--------|------|-------------|
| UP     | 14   | D5 |
| DOWN   | 12   | D6 |
| ENTER  | 13   | D7 |
| BUZZER | 15   | D8 |

---

### OLED I2C Wiring

| OLED Pin | NodeMCU Pin | GPIO |
|---------|-------------|------|
| VCC     | 3.3V        | — |
| GND     | GND         | — |
| SDA     | D2          | GPIO4 |
| SCL     | D1          | GPIO5 |

---

## Arduino IDE Setup

### ESP8266 Board Support
Add the following URL in:
File → Preferences → Additional Boards Manager URLs → then paste the URL → 
http://arduino.esp8266.com/stable/package_esp8266com_index.json



Install "ESP8266 by ESP8266 Community" from Boards Manager.

---

### Required Libraries
Install via Library Manager:
- Adafruit GFX Library
- Adafruit SSD1306

---

### Recommended Board Settings
- Board: NodeMCU 1.0 (ESP-12E Module)
- CPU Frequency: 80 MHz
- Flash Size: 4MB
- Upload Speed: 115200
- Erase Flash: Only Sketch

---

### Flashing
Open the main sketch:
src/ProjectileMachine_OLED/ProjectileMachine_OLED.ino


Select the correct COM port and upload.

---

## License

MIT License. See the LICENSE file for details.

---

## Author Note

This project was designed and built by **Majharul Islam Jihad** as a long-term learning and experimentation effort in embedded systems, physics simulation, and low-level optimization on constrained hardware.

The original plan was simple: visualize projectile motion on a small OLED.  
During development, the scope gradually expanded to include real-time physics accuracy, live trajectory prediction, velocity vector visualization, camera-follow effects, and realistic collision handling. Achieving this on an ESP8266 with limited memory and a 128×64 display required repeated redesigns, debugging sessions, and performance tuning.

A major challenge was maintaining numerical accuracy while keeping the code stable and responsive. Multiple iterations were needed to fix timing drift, ground-impact errors, prediction mismatches, and integration instability. Several approaches were tested and discarded before reaching the current reliable implementation.

AI assistance was used as a **technical aid**, not as a replacement for understanding. The AI helped with:
- validating physics equations
- identifying logical bugs
- suggesting safer numerical approaches
- improving code structure and documentation

All final design decisions, testing, debugging, and integration were done manually, with a focus on learning and correctness rather than speed.

This project represents persistence, problem-solving under constraint, and the practical use of AI as a supportive tool in engineering—not as an automated solution generator.
