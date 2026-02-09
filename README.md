# ProjectileMachine_OLED

A real-time, physics-accurate projectile motion simulator running entirely on an ESP8266 NodeMCU with a 128×64 SSD1306 OLED display.

This project demonstrates how mathematically correct motion simulation and smooth visualization can be achieved on constrained embedded hardware without relying on pre-rendered animations or lookup tables.

All trajectories are computed live using classical mechanics equations.

---

## Demo Video

https://youtube.com/shorts/ClX_AxegJIg?si=9OpkDlyONRSiR_SK

---

## Overview

ProjectileMachine_OLED is designed as a compact embedded physics engine.

Instead of drawing fake animations, the system performs real-time numerical calculations for projectile motion based on:

- initial height
- launch angle
- initial velocity
- gravity

Every launch produces a physically accurate trajectory generated directly by the microcontroller.

The focus of this project is:

- physics correctness
- numerical stability
- clean minimal UI
- smooth performance on limited memory hardware

---

## Key Features

- Real-time projectile motion simulation
- No precomputed or hardcoded paths
- Live trajectory prediction before launch
- Adjustable initial height
- Gravity presets:
  - Earth (9.81 m/s²)
  - Moon (1.62 m/s²)
  - Custom gravity (Morse-based numeric input)
- Decimal numeric input using Morse signals
- Velocity vector visualization (Vx, Vy)
- Camera-follow scrolling ground system
- Motion trail rendering
- Minimal 128×64 OLED interface
- Optional buzzer feedback
- Runs fully offline on ESP8266

---

## Physics Model

Standard projectile equations with initial height:

x(t) = v0 · cos(θ) · t  
y(t) = h0 + v0 · sin(θ) · t − ½ · g · t²  

Implementation details:

- fixed time-step integration
- gravity applied only to vertical velocity
- accurate ground-impact interpolation
- no lookup tables
- no animation shortcuts

All computations are performed in real time.

---

## User Interaction Flow

1. Power on → startup animation  
2. Select initial height  
3. Choose gravity (Earth / Moon / Custom)  
4. Adjust angle with live trajectory preview  
5. Adjust velocity  
6. Launch simulation  
7. View results (range, max height, flight time)

The interface is intentionally minimal to keep the display readable on a 128×64 screen.

---

## Hardware Requirements

- ESP8266 NodeMCU (ESP-12E / ESP-12F)
- SSD1306 OLED 128×64 (I2C)
- 3 push buttons
- Optional active buzzer
- Breadboard and jumper wires
- USB cable

---

## Wiring

### Buttons (INPUT_PULLUP)

| Control | GPIO | NodeMCU Pin |
|--------|------|-------------|
| UP     | 14   | D5 |
| DOWN   | 12   | D6 |
| ENTER  | 13   | D7 |
| BUZZER | 15   | D8 |

Buttons connect GPIO → GND (pressed = LOW).

### OLED (I2C)

| OLED Pin | NodeMCU |
|-----------|-----------|
| VCC | 3V3 |
| GND | GND |
| SDA | D2 (GPIO4) |
| SCL | D1 (GPIO5) |

---

## Arduino IDE Setup

### Board Support
Add:

http://arduino.esp8266.com/stable/package_esp8266com_index.json

Install:
ESP8266 by ESP8266 Community

### Libraries
- Adafruit GFX
- Adafruit SSD1306

### Recommended Settings
- Board: NodeMCU 1.0 (ESP-12E Module)
- CPU: 80 MHz
- Flash: 4MB
- Upload Speed: 115200

---

## Build

Open:

src/ProjectileMachine_OLED/ProjectileMachine_OLED.ino

Select the correct COM port and upload.

---

## Project Structure

src/ProjectileMachine_OLED/   main firmware  
docs/                        diagrams and media  
README.md                    documentation  

---

## Goals of This Project

This project was built as a learning and engineering exercise to explore:

- embedded systems programming
- real-time numerical simulation
- low-memory optimization
- physics visualization
- UI design for small displays

AI tools were used only as technical assistance for validation and debugging.  
All system design, implementation, and testing were performed manually.

---

## Author

Majharul Islam Jihad

GitHub:
https://github.com/majharulislamjihad2009-a11y

---

## License

MIT License
