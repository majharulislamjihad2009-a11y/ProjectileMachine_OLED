/**
 * Morse code input for custom gravity values
 */

#ifndef MORSE_H
#define MORSE_H

#include <Arduino.h>

class MorseInput {
  public:
    void begin();
    void addSymbol(char symbol); // '.' or '-'
    char confirmSymbol(); // Returns character or '\0' if invalid
    
    const char* getCurrentSequence() { return sequence; }
    
  private:
    char sequence[8]; // Current Morse sequence
    int seqPos;
    
    char decodeSequence();
    void resetSequence();
};

#endif