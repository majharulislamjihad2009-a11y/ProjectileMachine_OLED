/**
 * Physics engine for projectile simulation
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include <Arduino.h>
#include "Config.h"

struct Point {
  float x;
  float y;
};

struct TrailPoint {
  float x;
  float y;
  int age;
};

class PhysicsEngine {
  public:
    void begin();
    
    // Setup
    void setParameters(float height, float gravity, float angle, float velocity);
    void startSimulation(float height, float gravity, float angle, float velocity);
    
    // Simulation
    void update(unsigned long currentTime);
    bool isSimulationComplete() { return simulationComplete; }
    
    // Getters
    Point getCurrentPosition() { return currentPos; }
    Point getCurrentVelocity() { return {vx, vy}; }
    TrailPoint* getTrail() { return trail; }
    int getTrailLength() { return trailLength; }
    Point* getPrediction() { return prediction; }
    int getPredictionPoints() { return predictionPoints; }
    
    float getMaxHeight() { return maxHeight; }
    float getTotalRange() { return totalRange; }
    float getFlightTime() { return flightTime; }
    
    // For dotted path
    float getCurrentTime() { return currentTime; }
    
  private:
    // Simulation state
    float g; // Gravity
    float h0; // Initial height
    float angle; // Launch angle (radians)
    float v0; // Initial velocity
    float vx0, vy0; // Initial velocities
    float currentTime;
    float dt;
    
    float vx, vy; // Current velocities
    Point currentPos;
    
    // Trail
    TrailPoint trail[MAX_TRAIL_POINTS];
    int trailLength;
    int trailIndex;
    
    // Bounce tracking
    int bounceCount;
    bool simulationComplete;
    
    // Results
    float maxHeight;
    float totalRange;
    float flightTime;
    
    // Prediction
    Point prediction[MAX_PREDICTION_POINTS];
    int predictionPoints;
    
    // Helper methods
    void calculatePrediction();
    void updateTrail();
    void stopSimulation();
};

#endif