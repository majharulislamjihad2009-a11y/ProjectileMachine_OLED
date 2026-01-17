/**
 * Button handling implementation
 */

#include "Buttons.h"
#include "Pins.h"

void Buttons::begin() {
  buttons[0] = {BUTTON_UP, HIGH, HIGH, false, 0, false, 0};
  buttons[1] = {BUTTON_DOWN, HIGH, HIGH, false, 0, false, 0};
  buttons[2] = {BUTTON_ENTER, HIGH, HIGH, false, 0, false, 0};
  
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_ENTER, INPUT_PULLUP);
  
  lastUpdate = millis();
}

void Buttons::update() {
  unsigned long now = millis();
  
  // Debounce check
  if (now - lastUpdate < DEBOUNCE_MS) {
    return;
  }
  
  lastUpdate = now;
  
  for (int i = 0; i < 3; i++) {
    bool reading = digitalRead(buttons[i].pin);
    
    // Update state
    buttons[i].last = buttons[i].current;
    buttons[i].current = reading;
    
    // Detect press (LOW due to PULLUP)
    if (buttons[i].current == LOW && buttons[i].last == HIGH) {
      buttons[i].pressed = true;
      buttons[i].pressTime = now;
      buttons[i].holdActive = false;
    } else if (buttons[i].current == HIGH) {
      buttons[i].pressed = false;
      buttons[i].holdActive = false;
    }
    
    // Detect hold
    if (buttons[i].current == LOW && now - buttons[i].pressTime > HOLD_START_MS) {
      buttons[i].holdActive = true;
    }
  }
}

bool Buttons::isPressed(uint8_t button) {
  uint8_t idx = getButtonIndex(button);
  if (idx == 255) return false;
  return buttons[idx].current == LOW;
}

bool Buttons::wasPressed(uint8_t button) {
  uint8_t idx = getButtonIndex(button);
  if (idx == 255) return false;
  
  if (buttons[idx].pressed) {
    buttons[idx].pressed = false; // Consume the press
    return true;
  }
  return false;
}

bool Buttons::isHeld(uint8_t button) {
  uint8_t idx = getButtonIndex(button);
  if (idx == 255) return false;
  
  if (buttons[idx].holdActive) {
    unsigned long now = millis();
    unsigned long repeatRate = HOLD_REPEAT_MS;
    
    // Accelerate after 1.2 seconds
    if (now - buttons[idx].pressTime > HOLD_ACCELERATE_MS) {
      repeatRate = HOLD_REPEAT_MS / 2;
    }
    
    if (now - buttons[idx].lastRepeat > repeatRate) {
      buttons[idx].lastRepeat = now;
      return true;
    }
  }
  return false;
}

bool Buttons::wasLongPress(uint8_t button) {
  uint8_t idx = getButtonIndex(button);
  if (idx == 255) return false;
  
  if (buttons[idx].pressed && millis() - buttons[idx].pressTime > LONG_PRESS_MS) {
    buttons[idx].pressed = false;
    return true;
  }
  return false;
}

uint8_t Buttons::getButtonIndex(uint8_t pin) {
  for (int i = 0; i < 3; i++) {
    if (buttons[i].pin == pin) {
      return i;
    }
  }
  return 255;
}