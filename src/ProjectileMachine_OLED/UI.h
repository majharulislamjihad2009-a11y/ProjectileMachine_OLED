/**
 * OLED display rendering
 */

#ifndef UI_H
#define UI_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Physics.h"

struct DottedPoint {
  float x;
  float y;
  bool active;
};

class UIRenderer {
  public:
    UIRenderer(Adafruit_SSD1306* disp, PhysicsEngine* phys);
    void begin();
    void render(uint8_t state);
    
    // State setters
    void setHeight(float height);
    void setGravityMenu(int position);
    void setMorseInput(const char* buffer, const char* sequence);
    void setAngle(float angle);
    void setVelocity(float velocity);
    void setCannonMouthPosition(float angle, float height);
    void setSimulationData(Point ballPos, Point velocity, TrailPoint* trail, int trailLen);
    void setResults(float maxHeight, float range, float time);
    
    // Animation
    void setBootAnimationPhase(unsigned int phase);
    
    // Dotted path
    void addDottedPathPoint(float x, float y);
    void clearDottedPath();
    
  private:
    Adafruit_SSD1306* display;
    PhysicsEngine* physics;
    
    // Current state data
    float currentHeight;
    int gravityMenuPos;
    const char* morseBuffer;
    const char* morseSequence;
    float currentAngle;
    float currentVelocity;
    Point simBallPos;
    Point simVelocity;
    TrailPoint* simTrail;
    int simTrailLen;
    float resultMaxHeight;
    float resultRange;
    float resultTime;
    
    // Cannon mouth position
    float cannonMouthX;
    float cannonMouthY;
    
    // Dotted path
    DottedPoint dottedPath[MAX_DOTTED_POINTS];
    int dottedPathCount;
    
    // Camera
    float cameraX;
    
    // Boot animation
    unsigned int bootAnimPhase;
    
    // Rendering methods
    void renderBootAnimation();
    void renderHeightSelect();
    void renderGravityMenu();
    void renderMorseInput();
    void renderAngleAdjust();
    void renderVelocityAdjust();
    void renderSimulation();
    void renderResults();
    
    // Helper methods
    void drawCannon(float angle, float mouthX, float mouthY);
    void drawGround(float offsetX);
    void drawPredictedPath(float startX, float startY);
    void drawDottedPath();
    void drawVelocityVectors(float x, float y, float vx, float vy);
    void drawHUD(const char* line1, const char* line2 = "");
    int worldToScreenX(float worldX);
    int worldToScreenY(float worldY);
    
    // Assets
    void drawRocket(int x, int y, int phase);
    void drawTitle();
    void drawStars(int count);
};

#endif