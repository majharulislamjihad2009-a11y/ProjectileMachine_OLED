/**
 * Morse code input implementation
 */

#include "Morse.h"

// Morse codes for digits 0-9 and decimal point
struct MorseCode {
  const char* pattern;
  char character;
};

const MorseCode morseTable[] = {
  {"-----", '0'},
  {".----", '1'},
  {"..---", '2'},
  {"...--", '3'},
  {"....-", '4'},
  {".....", '5'},
  {"-....", '6'},
  {"--...", '7'},
  {"---..", '8'},
  {"----.", '9'},
  {".-", '.'}, // Decimal point
  {" ", ' '}  // Space (complete input)
};

void MorseInput::begin() {
  resetSequence();
}

void MorseInput::addSymbol(char symbol) {
  if (seqPos < 6) { // Max sequence length is 5 symbols + null terminator
    sequence[seqPos++] = symbol;
    sequence[seqPos] = '\0';
  }
}

char MorseInput::confirmSymbol() {
  if (seqPos == 0) {
    // Empty sequence means space (complete input)
    return ' ';
  }
  
  char result = decodeSequence();
  resetSequence();
  return result;
}

char MorseInput::decodeSequence() {
  // Check for exact matches
  for (int i = 0; i < sizeof(morseTable) / sizeof(morseTable[0]); i++) {
    if (strcmp(sequence, morseTable[i].pattern) == 0) {
      return morseTable[i].character;
    }
  }
  
  // Check for space (used to complete input)
  if (strcmp(sequence, " ") == 0) {
    return ' ';
  }
  
  return '\0'; // Invalid sequence
}

void MorseInput::resetSequence() {
  sequence[0] = '\0';
  seqPos = 0;
}