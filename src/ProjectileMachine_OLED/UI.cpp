/**
 * OLED rendering implementation
 */

#include "UI.h"
#include "Config.h"
#include "Assets.h"

UIRenderer::UIRenderer(Adafruit_SSD1306* disp, PhysicsEngine* phys) {
  display = disp;
  physics = phys;
  cameraX = 0;
  cannonMouthX = CANNON_X + CANNON_LENGTH;
  cannonMouthY = GROUND_Y - CANNON_LENGTH;
  dottedPathCount = 0;
  bootAnimPhase = 0;
  
  // Initialize dotted path
  for (int i = 0; i < MAX_DOTTED_POINTS; i++) {
    dottedPath[i].active = false;
  }
}

void UIRenderer::begin() {
  display->clearDisplay();
  display->setTextColor(SSD1306_WHITE);
  display->setTextSize(1);
}

void UIRenderer::render(uint8_t state) {
  display->clearDisplay();
  
  switch (state) {
    case 0: // BOOT_ANIM
      renderBootAnimation();
      break;
    case 1: // HEIGHT_SELECT
      renderHeightSelect();
      break;
    case 2: // GRAVITY_MENU
      renderGravityMenu();
      break;
    case 3: // MORSE_INPUT
      renderMorseInput();
      break;
    case 4: // ANGLE_ADJUST
      renderAngleAdjust();
      break;
    case 5: // VELOCITY_ADJUST
      renderVelocityAdjust();
      break;
    case 6: // SIMULATION_RUN
      renderSimulation();
      break;
    case 7: // RESULTS
      renderResults();
      break;
  }
  
  display->display();
}

void UIRenderer::setHeight(float height) { currentHeight = height; }
void UIRenderer::setGravityMenu(int position) { gravityMenuPos = position; }
void UIRenderer::setMorseInput(const char* buffer, const char* sequence) {
  morseBuffer = buffer;
  morseSequence = sequence;
}
void UIRenderer::setAngle(float angle) { currentAngle = angle; }
void UIRenderer::setVelocity(float velocity) { currentVelocity = velocity; }
void UIRenderer::setCannonMouthPosition(float angle, float height) {
  float angleRad = angle * M_PI / 180.0f;
  cannonMouthX = CANNON_X + CANNON_LENGTH * cos(angleRad);
  cannonMouthY = GROUND_Y - CANNON_LENGTH * sin(angleRad) - height;
}
void UIRenderer::setSimulationData(Point ballPos, Point velocity, TrailPoint* trail, int trailLen) {
  simBallPos = ballPos;
  simVelocity = velocity;
  simTrail = trail;
  simTrailLen = trailLen;
}
void UIRenderer::setResults(float maxHeight, float range, float time) {
  resultMaxHeight = maxHeight;
  resultRange = range;
  resultTime = time;
}
void UIRenderer::setBootAnimationPhase(unsigned int phase) {
  bootAnimPhase = phase;
}

void UIRenderer::addDottedPathPoint(float x, float y) {
  if (dottedPathCount < MAX_DOTTED_POINTS) {
    dottedPath[dottedPathCount].x = x;
    dottedPath[dottedPathCount].y = y;
    dottedPath[dottedPathCount].active = true;
    dottedPathCount++;
  } else {
    // Shift array to make room
    for (int i = 0; i < MAX_DOTTED_POINTS - 1; i++) {
      dottedPath[i] = dottedPath[i + 1];
    }
    dottedPath[MAX_DOTTED_POINTS - 1].x = x;
    dottedPath[MAX_DOTTED_POINTS - 1].y = y;
    dottedPath[MAX_DOTTED_POINTS - 1].active = true;
  }
}

void UIRenderer::clearDottedPath() {
  for (int i = 0; i < MAX_DOTTED_POINTS; i++) {
    dottedPath[i].active = false;
  }
  dottedPathCount = 0;
}

void UIRenderer::renderBootAnimation() {
  // Draw stars in background
  drawStars(15);
  
  // Draw animated rocket
  int rocketX = 64;
  int rocketY = 32 - (bootAnimPhase * 32 / 24);
  drawRocket(rocketX, rocketY, bootAnimPhase);
  
  // Draw title with fade-in effect
  if (bootAnimPhase > 8) {
    drawTitle();
  }
  
  // Draw progress bar at bottom
  int progress = min(100, (int)((bootAnimPhase * 100) / 24));
  int barWidth = (progress * 100) / 100;
  display->drawRect(14, 52, 100, 6, SSD1306_WHITE);
  display->fillRect(15, 53, barWidth, 4, SSD1306_WHITE);
}

void UIRenderer::renderHeightSelect() {
  // Draw ground
  drawGround(0);
  
  // Draw cannon at default position
  display->fillRect(CANNON_X - 2, GROUND_Y - 2, 4, 4, SSD1306_WHITE);
  
  // Draw height indicator
  int heightY = GROUND_Y - (currentHeight * 2); // Scale for visibility
  display->fillCircle(CANNON_X + CANNON_LENGTH, heightY, 3, SSD1306_WHITE);
  
  // Draw dashed line from cannon to height
  for (int y = CANNON_Y; y > heightY; y -= 4) {
    display->drawPixel(CANNON_X + CANNON_LENGTH, y, SSD1306_WHITE);
  }
  
  // HUD
  char buf[16];
  dtostrf(currentHeight, 4, 1, buf);
  drawHUD("Height:", buf);
  
  // Instructions
  display->setCursor(10, 0);
  display->print(F(""));
  display->setCursor(20, 8);
  display->print(F(""));
}

void UIRenderer::renderGravityMenu() {
  display->setCursor(40, 10);
  display->print(F("GRAVITY"));
  
  // Draw selection indicators
  for (int i = 0; i < 3; i++) {
    int y = 25 + i * 12;
    
    if (i == gravityMenuPos) {
      display->fillTriangle(20, y, 20, y + 8, 25, y + 4, SSD1306_WHITE);
    } else {
      display->drawTriangle(20, y, 20, y + 8, 25, y + 4, SSD1306_WHITE);
    }
    
    display->setCursor(30, y);
    switch (i) {
      case 0: display->print(F("Earth=9.81")); break;
      case 1: display->print(F("Moon=1.62")); break;
      case 2: display->print(F("Custom")); break;
    }
  }
}

void UIRenderer::renderMorseInput() {
  display->setCursor(40, 5);
  display->print(F("CUSTOM G"));
  
  // Display current value
  display->setCursor(10, 20);
  display->print(F("g="));
  display->print(morseBuffer);
  
  // Display current Morse sequence
  display->setCursor(10, 35);
  display->print(F("Morse: "));
  display->print(morseSequence);
  
  // Instructions
  display->setCursor(5, 50);
  display->print(F(".- = Decimal"));
  display->setCursor(20, 58);
  display->print(F(""));
}

void UIRenderer::renderAngleAdjust() {
  // Draw ground
  drawGround(0);
  
  // Draw cannon with current angle
  display->fillRect(CANNON_X - 2, GROUND_Y - 2, 4, 4, SSD1306_WHITE);
  
  // Draw predicted path starting from cannon mouth
  drawPredictedPath(cannonMouthX, cannonMouthY);
  
  // HUD
  char buf[16];
  dtostrf(currentAngle, 4, 1, buf);
  drawHUD("Angle:", buf);
  
  // Instructions
  display->setCursor(10, 0);
  display->print(F(""));
  display->setCursor(30, 8);
  display->print(F("ENTER:"));
}

void UIRenderer::renderVelocityAdjust() {
  // Draw ground
  drawGround(0);
  
  // Draw cannon with current angle
  drawCannon(currentAngle, cannonMouthX, cannonMouthY);
  
  // Draw predicted path starting from cannon mouth
  drawPredictedPath(cannonMouthX, cannonMouthY);
  
  // HUD
  char buf[16];
  dtostrf(currentVelocity, 4, 1, buf);
  drawHUD("Velocity:", buf);
  
  // Instructions
  display->setCursor(10, 0);
  display->print(F(""));
  display->setCursor(30, 8);
  display->print(F("ENTER:"));
}

void UIRenderer::renderSimulation() {
  // Update camera to follow ball
  cameraX = simBallPos.x - SCREEN_WIDTH / 2;
  if (cameraX < 0) cameraX = 0;
  
  // Draw ground with scrolling
  drawGround(cameraX);
  
  // Draw cannon (moves with ground scroll) - start from CANNON_X position
  int cannonScreenX = CANNON_X - cameraX;
  if (cannonScreenX > -20 && cannonScreenX < SCREEN_WIDTH) {
    // Draw cannon base at CANNON_X position
    display->fillRect(cannonScreenX - 3, GROUND_Y - 2, 6, 4, SSD1306_WHITE);
    // Draw cannon barrel (simplified) starting from CANNON_X
    display->drawLine(cannonScreenX, GROUND_Y, 
                     cannonScreenX + CANNON_LENGTH, 
                     GROUND_Y - CANNON_LENGTH, 
                     SSD1306_WHITE);
  }
  
  // Draw dotted path - starting from CANNON_X position
  for (int i = 0; i < dottedPathCount; i += 2) { // Draw every other point
    if (dottedPath[i].active) {
      // Add CANNON_X offset to make path start from cannon
      int screenX = CANNON_X + dottedPath[i].x - cameraX;
      int screenY = GROUND_Y - dottedPath[i].y;
      
      if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        display->drawPixel(screenX, screenY, SSD1306_WHITE);
      }
    }
  }
  
  // Draw trail - starting from CANNON_X position
  for (int i = 0; i < simTrailLen; i++) {
    if (simTrail[i].age < 255) {
      int alpha = 255 - simTrail[i].age * 20;
      if (alpha > 30) {
        // Add CANNON_X offset to make trail start from cannon
        int x = CANNON_X + simTrail[i].x - cameraX;
        int y = GROUND_Y - simTrail[i].y;
        if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
          display->drawPixel(x, y, SSD1306_WHITE);
        }
      }
    }
  }
  
  // Draw ball - starting from CANNON_X position
  // This is the key fix: Add CANNON_X offset to the ball's position
  int ballX = CANNON_X + simBallPos.x - cameraX;
  int ballY = GROUND_Y - simBallPos.y;
  display->fillCircle(ballX, ballY, BALL_RADIUS, SSD1306_WHITE);
  
  // Draw velocity vectors attached to ball
  drawVelocityVectors(ballX, ballY, simVelocity.x, simVelocity.y);
  
  // Draw HUD with time
  char buf[16];
  dtostrf(simBallPos.x, 5, 1, buf);
  drawHUD("X:", buf);
}

void UIRenderer::renderResults() {
  display->setCursor(40, 5);
  display->print(F("RESULTS"));
  
  display->setCursor(10, 20);
  display->print(F("Range:"));
  display->setCursor(70, 20);
  display->print(resultRange, 1);
  display->print(F("m"));
  
  display->setCursor(10, 30);
  display->print(F("Max H:"));
  display->setCursor(70, 30);
  display->print(resultMaxHeight, 1);
  display->print(F("m"));
  
  display->setCursor(10, 40);
  display->print(F("Time:"));
  display->setCursor(70, 40);
  display->print(resultTime, 1);
  display->print(F("s"));
  
  display->setCursor(10, 55);
  display->print(F("ENTER:restart"));
}

void UIRenderer::drawCannon(float angle, float mouthX, float mouthY) {
  float angleRad = angle * M_PI / 180.0f;
  
  // Calculate cannon base position (fixed to ground)
  int baseX = CANNON_X;
  int baseY = GROUND_Y;
  
  // Calculate mouth position
  int mouthScreenX = mouthX;
  int mouthScreenY = mouthY;
  
  // Draw cannon base
  display->fillRect(baseX - 3, baseY - 2, 6, 4, SSD1306_WHITE);
  
  // Draw cannon barrel
  display->drawLine(baseX, baseY, mouthScreenX, mouthScreenY, SSD1306_WHITE);
  display->drawLine(baseX, baseY - 1, mouthScreenX, mouthScreenY - 1, SSD1306_WHITE);
  
  // Draw cannon mouth (highlight)
  display->fillCircle(mouthScreenX, mouthScreenY, 2, SSD1306_WHITE);
}

void UIRenderer::drawGround(float offsetX) {
  int groundY = GROUND_Y;
  
  // Draw ground line
  display->drawLine(0, groundY, SCREEN_WIDTH, groundY, SSD1306_WHITE);
  
  // Draw ground texture (dots)
  for (int x = ((int)offsetX % 8); x < SCREEN_WIDTH; x += 8) {
    display->drawPixel(x, groundY + 1, SSD1306_WHITE);
    display->drawPixel(x + 4, groundY + 2, SSD1306_WHITE);
  }
}

void UIRenderer::drawPredictedPath(float startX, float startY) {
  // Get prediction points from physics engine
  Point* prediction = physics->getPrediction();
  int points = physics->getPredictionPoints();
  
  if (points < 2) return;
  
  // Draw predicted path as dots starting from cannon mouth
  for (int i = 0; i < points; i += 2) { // Draw every other point for dotted effect
    // Add CANNON_X offset to start from cannon mouth
    int screenX = CANNON_X + prediction[i].x;
    int screenY = GROUND_Y - prediction[i].y;
    
    if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
      display->drawPixel(screenX, screenY, SSD1306_WHITE);
    }
  }
}

void UIRenderer::drawDottedPath() {
  // Draw dotted path from stored points
  for (int i = 0; i < dottedPathCount; i += 2) { // Draw every other point
    if (dottedPath[i].active) {
      // Add CANNON_X offset to make path start from cannon
      int screenX = CANNON_X + dottedPath[i].x - cameraX;
      int screenY = GROUND_Y - dottedPath[i].y;
      
      if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT) {
        display->drawPixel(screenX, screenY, SSD1306_WHITE);
      }
    }
  }
}

void UIRenderer::drawVelocityVectors(float ballX, float ballY, float vx, float vy) {
  // Scale factors
  float scale = VECTOR_SCALE;
  float maxLength = MAX_VECTOR_LENGTH;
  
  // Calculate arrow lengths
  float vxLen = constrain(abs(vx) * scale, 0, maxLength);
  float vyLen = constrain(abs(vy) * scale, 0, maxLength);
  
  // Draw horizontal component (always to the right)
  int startX = ballX;
  int startY = ballY;
  
  // Draw Vx arrow
  int vxEndX = startX + vxLen;
  display->drawLine(startX, startY, vxEndX, startY, SSD1306_WHITE);
  // Arrowhead for Vx
  display->drawLine(vxEndX, startY, vxEndX - VECTOR_ARROW_SIZE, startY - VECTOR_ARROW_SIZE, SSD1306_WHITE);
  display->drawLine(vxEndX, startY, vxEndX - VECTOR_ARROW_SIZE, startY + VECTOR_ARROW_SIZE, SSD1306_WHITE);
  
  // Draw Vy arrow (direction depends on sign of vy)
  int vyEndY = startY - (vy * scale); // Note: screen Y increases downward, so subtract for upward
  
  if (vy > 0) { // Ball moving upward
    display->drawLine(startX, startY, startX, vyEndY, SSD1306_WHITE);
    // Arrowhead pointing upward
    display->drawLine(startX, vyEndY, startX - VECTOR_ARROW_SIZE, vyEndY + VECTOR_ARROW_SIZE, SSD1306_WHITE);
    display->drawLine(startX, vyEndY, startX + VECTOR_ARROW_SIZE, vyEndY + VECTOR_ARROW_SIZE, SSD1306_WHITE);
  } else { // Ball moving downward
    display->drawLine(startX, startY, startX, vyEndY, SSD1306_WHITE);
    // Arrowhead pointing downward
    display->drawLine(startX, vyEndY, startX - VECTOR_ARROW_SIZE, vyEndY - VECTOR_ARROW_SIZE, SSD1306_WHITE);
    display->drawLine(startX, vyEndY, startX + VECTOR_ARROW_SIZE, vyEndY - VECTOR_ARROW_SIZE, SSD1306_WHITE);
  }
  
  // Draw small labels if there's space
  if (ballX < SCREEN_WIDTH - 20 && ballY > 15) {
    display->setCursor(startX + 2, startY - 12);
    display->print(F(""));
    display->setCursor(startX + 2, startY - 4);
    display->print(F(""));
  }
}

void UIRenderer::drawHUD(const char* line1, const char* line2) {
  display->setCursor(SCREEN_WIDTH - 50, 0);
  display->print(line1);
  display->setCursor(SCREEN_WIDTH - 50, 8);
  display->print(line2);
}

int UIRenderer::worldToScreenX(float worldX) {
  return worldX + CANNON_X - cameraX;
}

int UIRenderer::worldToScreenY(float worldY) {
  return cannonMouthY - worldY;
}

void UIRenderer::drawRocket(int x, int y, int phase) {
  // Animated rocket with flame effect
  int flameSize = (phase % 8) / 2; // Pulsing flame
  
  // Rocket body
  display->fillTriangle(x, y, x - 4, y + 8, x + 4, y + 8, SSD1306_WHITE);
  display->fillRect(x - 2, y + 8, 4, 12, SSD1306_WHITE);
  display->fillTriangle(x - 2, y + 20, x + 2, y + 20, x, y + 25, SSD1306_WHITE);
  
  // Flame with animation
  for (int i = 0; i < 3 + flameSize; i++) {
    int offset = i - (1 + flameSize/2);
    display->drawPixel(x + offset, y + 21 + flameSize, SSD1306_WHITE);
    if (flameSize > 0) {
      display->drawPixel(x + offset, y + 22 + flameSize, SSD1306_WHITE);
    }
  }
  
  // Windows
  display->fillCircle(x, y + 10, 1, SSD1306_BLACK);
  display->drawCircle(x, y + 10, 1, SSD1306_WHITE);
}

void UIRenderer::drawTitle() {
  // Fancy title with physics symbols
  display->setCursor(25, 45);
  display->print(F(""));
  display->setCursor(35, 53);
  display->print(F(""));
  
  // Draw small trajectory curve
  for (int i = 0; i < 20; i++) {
    int x = 10 + i * 5;
    int y = 40 - (i * i) / 20;
    display->drawPixel(x, y, SSD1306_WHITE);
  }
}

void UIRenderer::drawStars(int count) {
  // Draw random stars in background
  for (int i = 0; i < count; i++) {
    int x = (i * 17) % SCREEN_WIDTH;
    int y = (i * 23) % (SCREEN_HEIGHT - 20);
    
    // Make some stars twinkle based on animation phase
    if ((bootAnimPhase + i) % 4 == 0) {
      display->drawPixel(x, y, SSD1306_WHITE);
    }
  }
}