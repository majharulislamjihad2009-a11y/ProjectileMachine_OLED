/**
 * Buzzer control for audio feedback
 */

#ifndef BEEP_H
#define BEEP_H

#include <Arduino.h>
#include "Config.h"

class Beep {
  public:
    void begin();
    void update();
    
    void beep(unsigned int duration);
    void startFlightBeep();
    void stopFlightBeep();
    
  private:
    bool active;
    unsigned long beepStart;
    unsigned int beepDuration;
    
    bool flightBeepActive;
    unsigned long lastFlightBeep;
};

#endif