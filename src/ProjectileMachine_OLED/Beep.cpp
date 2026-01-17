/**
 * Buzzer control implementation
 */

#include "Beep.h"
#include "Pins.h"

void Beep::begin() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  active = false;
  flightBeepActive = false;
  beepStart = 0;
  beepDuration = 0;
  lastFlightBeep = 0;
}

void Beep::update() {
  unsigned long now = millis();
  
  // Handle single beep
  if (active && now - beepStart >= beepDuration) {
    digitalWrite(BUZZER_PIN, LOW);
    active = false;
  }
  
  // Handle flight beep
  if (flightBeepActive && now - lastFlightBeep >= FLIGHT_BEEP_INTERVAL) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(500);
    digitalWrite(BUZZER_PIN, LOW);
    lastFlightBeep = now;
  }
}

void Beep::beep(unsigned int duration) {
  if (!BEEPER_ENABLED) return;
  
  digitalWrite(BUZZER_PIN, HIGH);
  active = true;
  beepStart = millis();
  beepDuration = duration;
}

void Beep::startFlightBeep() {
  if (!BEEPER_ENABLED) return;
  
  flightBeepActive = true;
  lastFlightBeep = millis();
}

void Beep::stopFlightBeep() {
  flightBeepActive = false;
  digitalWrite(BUZZER_PIN, LOW);
}