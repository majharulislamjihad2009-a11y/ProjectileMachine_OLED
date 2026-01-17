/**
 * ProjectileMachine_OLED
 * Main sketch file for ESP8266 projectile simulation
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"
#include "Pins.h"
#include "Buttons.h"
#include "Beep.h"
#include "Morse.h"
#include "Physics.h"
#include "UI.h"
#include "Assets.h"

// Global objects
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
Buttons buttons;
Beep buzzer;
MorseInput morse;
PhysicsEngine physics;
UIRenderer ui(&display, &physics);

// State machine
enum AppState {
  STATE_BOOT_ANIM,
  STATE_HEIGHT_SELECT,
  STATE_GRAVITY_MENU,
  STATE_MORSE_INPUT,
  STATE_ANGLE_ADJUST,
  STATE_VELOCITY_ADJUST,
  STATE_SIMULATION_RUN,
  STATE_RESULTS
};

AppState currentState = STATE_BOOT_ANIM;
AppState prevState = STATE_BOOT_ANIM;
unsigned long stateEnterTime = 0;
unsigned long lastFrameTime = 0;

// Input handling
int buttonAction = 0; // 0=none, 1=up, 2=down, 3=enter, 4=long_enter

// Simulation parameters
float initialHeight = 0.0f;
float gravity = 9.81f;
float launchAngle = 45.0f;
float launchVelocity = 20.0f;

// Menu positions
int gravityMenuPos = 0; // 0=Earth, 1=Moon, 2=Custom

// Morse input buffer
char morseInputBuffer[16] = "";

void setup() {
  Serial.begin(115200);
  Serial.println(F("ProjectileMachine_OLED starting..."));
  
  // Initialize OLED
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();
  
  // Initialize components
  buttons.begin();
  buzzer.begin();
  physics.begin();
  ui.begin();
  
  // Start with boot animation
  enterState(STATE_BOOT_ANIM);
  
  Serial.println(F("Initialization complete"));
}

void loop() {
  unsigned long now = millis();
  
  // Update buttons
  buttons.update();
  buttonAction = 0;
  
  // Check for button presses
  if (buttons.wasPressed(BUTTON_UP)) {
    buttonAction = 1;
    buzzer.beep(BEEP_SHORT);
  } else if (buttons.wasPressed(BUTTON_DOWN)) {
    buttonAction = 2;
    buzzer.beep(BEEP_SHORT);
  } else if (buttons.wasPressed(BUTTON_ENTER)) {
    buttonAction = 3;
    buzzer.beep(BEEP_MEDIUM);
  } else if (buttons.wasLongPress(BUTTON_ENTER)) {
    buttonAction = 4;
    buzzer.beep(BEEP_LONG);
  }
  
  // Handle button hold repeat
  if (buttons.isHeld(BUTTON_UP)) {
    buttonAction = 1;
  } else if (buttons.isHeld(BUTTON_DOWN)) {
    buttonAction = 2;
  }
  
  // State machine
  switch (currentState) {
    case STATE_BOOT_ANIM:
      stateBootAnimation(now);
      break;
    case STATE_HEIGHT_SELECT:
      stateHeightSelect();
      break;
    case STATE_GRAVITY_MENU:
      stateGravityMenu();
      break;
    case STATE_MORSE_INPUT:
      stateMorseInput();
      break;
    case STATE_ANGLE_ADJUST:
      stateAngleAdjust();
      break;
    case STATE_VELOCITY_ADJUST:
      stateVelocityAdjust();
      break;
    case STATE_SIMULATION_RUN:
      stateSimulationRun(now);
      break;
    case STATE_RESULTS:
      stateResults();
      break;
  }
  
  // Render at target FPS
  if (now - lastFrameTime >= FRAME_TIME_MS) {
    ui.render(currentState);
    lastFrameTime = now;
  }
  
  // Update buzzer
  buzzer.update();
}

void enterState(AppState newState) {
  prevState = currentState;
  currentState = newState;
  stateEnterTime = millis();
  Serial.print(F("Entering state: "));
  Serial.println(newState);
  
  // State-specific initialization
  switch (newState) {
    case STATE_HEIGHT_SELECT:
      initialHeight = 0.0f;
      break;
    case STATE_GRAVITY_MENU:
      gravityMenuPos = 0;
      break;
    case STATE_MORSE_INPUT:
      morse.begin();
      strcpy(morseInputBuffer, "");
      break;
    case STATE_ANGLE_ADJUST:
      launchAngle = 45.0f;
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case STATE_VELOCITY_ADJUST:
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case STATE_SIMULATION_RUN:
      physics.startSimulation(initialHeight, gravity, launchAngle, launchVelocity);
      buzzer.startFlightBeep();
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
  }
}

void stateBootAnimation(unsigned long now) {
  static unsigned int animPhase = 0;
  static unsigned long lastAnimUpdate = 0;
  
  unsigned long elapsed = now - stateEnterTime;
  
  if (elapsed > BOOT_ANIM_DURATION) {
    enterState(STATE_HEIGHT_SELECT);
    return;
  }
  
  // Update animation every 50ms
  if (now - lastAnimUpdate > 50) {
    animPhase = (animPhase + 1) % 24;
    lastAnimUpdate = now;
    ui.setBootAnimationPhase(animPhase);
  }
}

void stateHeightSelect() {
  switch (buttonAction) {
    case 1: // UP
      initialHeight = min(initialHeight + HEIGHT_STEP, MAX_HEIGHT);
      break;
    case 2: // DOWN
      initialHeight = max(initialHeight - HEIGHT_STEP, MIN_HEIGHT);
      break;
    case 3: // ENTER
    case 4: // LONG ENTER
      enterState(STATE_GRAVITY_MENU);
      break;
  }
  
  // Update UI
  ui.setHeight(initialHeight);
}

void stateGravityMenu() {
  switch (buttonAction) {
    case 1: // UP
      gravityMenuPos = (gravityMenuPos + 2) % 3; // Move up (0->2, 1->0, 2->1)
      buzzer.beep(BEEP_SHORT);
      break;
    case 2: // DOWN
      gravityMenuPos = (gravityMenuPos + 1) % 3; // Move down
      buzzer.beep(BEEP_SHORT);
      break;
    case 3: // ENTER
      buzzer.beep(BEEP_MEDIUM);
      switch (gravityMenuPos) {
        case 0: // Earth
          gravity = EARTH_GRAVITY;
          enterState(STATE_ANGLE_ADJUST);
          break;
        case 1: // Moon
          gravity = MOON_GRAVITY;
          enterState(STATE_ANGLE_ADJUST);
          break;
        case 2: // Custom
          enterState(STATE_MORSE_INPUT);
          break;
      }
      break;
  }
  
  ui.setGravityMenu(gravityMenuPos);
}

void stateMorseInput() {
  switch (buttonAction) {
    case 1: // UP (dot)
      morse.addSymbol('.');
      buzzer.beep(BEEP_SHORT);
      break;
    case 2: // DOWN (dash)
      morse.addSymbol('-');
      buzzer.beep(BEEP_MEDIUM);
      break;
    case 3: // ENTER (confirm symbol/space)
      {
        char result = morse.confirmSymbol();
        if (result != '\0') {
          if (result == ' ') {
            // Space completes input
            float customGravity = atof(morseInputBuffer);
            if (customGravity >= 0.1f && customGravity <= 20.0f) {
              gravity = customGravity;
              enterState(STATE_ANGLE_ADJUST);
            } else {
              buzzer.beep(BEEP_ERROR);
              strcpy(morseInputBuffer, "");
              morse.begin();
            }
          } else {
            // Append character
            int len = strlen(morseInputBuffer);
            if (len < 15) {
              morseInputBuffer[len] = result;
              morseInputBuffer[len + 1] = '\0';
            }
          }
        }
      }
      break;
    case 4: // LONG ENTER (clear)
      strcpy(morseInputBuffer, "");
      morse.begin();
      buzzer.beep(BEEP_ERROR);
      break;
  }
  
  ui.setMorseInput(morseInputBuffer, morse.getCurrentSequence());
}

void stateAngleAdjust() {
  switch (buttonAction) {
    case 1: // UP
      launchAngle = min(launchAngle + ANGLE_STEP, MAX_ANGLE);
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case 2: // DOWN
      launchAngle = max(launchAngle - ANGLE_STEP, MIN_ANGLE);
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case 3: // ENTER
      enterState(STATE_VELOCITY_ADJUST);
      break;
  }
  
  ui.setAngle(launchAngle);
}

void stateVelocityAdjust() {
  switch (buttonAction) {
    case 1: // UP
      launchVelocity = min(launchVelocity + VELOCITY_STEP, MAX_VELOCITY);
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case 2: // DOWN
      launchVelocity = max(launchVelocity - VELOCITY_STEP, MIN_VELOCITY);
      physics.setParameters(initialHeight, gravity, launchAngle, launchVelocity);
      ui.setCannonMouthPosition(launchAngle, initialHeight);
      break;
    case 3: // ENTER
      enterState(STATE_SIMULATION_RUN);
      break;
  }
  
  ui.setVelocity(launchVelocity);
}

void stateSimulationRun(unsigned long now) {
  static bool pathDrawn[MAX_PREDICTION_POINTS] = {false};
  static int currentPathIndex = 0;
  
  physics.update(now);
  
  // Update dotted path
  Point currentPos = physics.getCurrentPosition();
  if (currentPathIndex < MAX_PREDICTION_POINTS - 1) {
    // Mark current position as part of the dotted path
    ui.addDottedPathPoint(currentPos.x, currentPos.y);
    currentPathIndex++;
  }
  
  if (physics.isSimulationComplete()) {
    buzzer.stopFlightBeep();
    enterState(STATE_RESULTS);
    // Reset path drawing for next simulation
    for (int i = 0; i < MAX_PREDICTION_POINTS; i++) {
      pathDrawn[i] = false;
    }
    currentPathIndex = 0;
    ui.clearDottedPath();
  }
  
  ui.setSimulationData(physics.getCurrentPosition(), 
                       physics.getCurrentVelocity(),
                       physics.getTrail(),
                       physics.getTrailLength());
}

void stateResults() {
  switch (buttonAction) {
    case 3: // ENTER
    case 4: // LONG ENTER
      enterState(STATE_HEIGHT_SELECT);
      break;
  }
  
  ui.setResults(physics.getMaxHeight(), 
                physics.getTotalRange(), 
                physics.getFlightTime());
}