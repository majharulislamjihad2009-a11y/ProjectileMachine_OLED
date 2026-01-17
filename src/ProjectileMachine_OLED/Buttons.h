/**
 * Button handling with debounce and hold detection
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "Config.h"

class Buttons {
  public:
    void begin();
    void update();
    
    bool isPressed(uint8_t button);
    bool wasPressed(uint8_t button);
    bool isHeld(uint8_t button);
    bool wasLongPress(uint8_t button);
    
  private:
    struct ButtonState {
      uint8_t pin;
      bool current;
      bool last;
      bool pressed;
      unsigned long pressTime;
      bool holdActive;
      unsigned long lastRepeat;
    };
    
    ButtonState buttons[3];
    unsigned long lastUpdate;
    
    uint8_t getButtonIndex(uint8_t pin);
};

#endif