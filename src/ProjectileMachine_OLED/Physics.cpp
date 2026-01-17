/**
 * Physics engine implementation
 */

#include "Physics.h"
#include <math.h>

void PhysicsEngine::begin() {
  simulationComplete = true;
  trailLength = 0;
  bounceCount = 0;
  maxHeight = 0;
  totalRange = 0;
  flightTime = 0;
}

void PhysicsEngine::setParameters(float height, float gravity, float angle, float velocity) {
  h0 = height;
  g = gravity;
  this->angle = angle * M_PI / 180.0f; // Convert to radians
  v0 = velocity;
  
  calculatePrediction();
}

void PhysicsEngine::startSimulation(float height, float gravity, float angle, float velocity) {
  h0 = height;
  g = gravity;
  this->angle = angle * M_PI / 180.0f;
  v0 = velocity;
  
  // Initialize simulation
  currentTime = 0;
  dt = SIMULATION_DT;
  
  // Calculate initial velocities
  float vx0 = v0 * cos(this->angle);
  float vy0 = v0 * sin(this->angle);
  
  vx = vx0;
  vy = vy0;
  
  currentPos.x = 0;
  currentPos.y = h0;
  
  // Initialize trail
  trailLength = 0;
  trailIndex = 0;
  for (int i = 0; i < MAX_TRAIL_POINTS; i++) {
    trail[i] = {0, 0, 0};
  }
  
  // Reset tracking
  bounceCount = 0;
  simulationComplete = false;
  maxHeight = h0;
  totalRange = 0;
  flightTime = 0;
}

void PhysicsEngine::update(unsigned long currentMillis) {
  if (simulationComplete) return;
  
  static unsigned long lastUpdate = 0;
  if (currentMillis - lastUpdate < FRAME_TIME_MS) {
    return;
  }
  lastUpdate = currentMillis;
  
  // Calculate current velocities using exact equations
  // vx remains constant (no air resistance)
  float vx0 = v0 * cos(angle);
  float vy0 = v0 * sin(angle);
  
  // Update time
  currentTime += dt;
  flightTime = currentTime;
  
  // EXACT EQUATIONS - no integration errors
  // x(t) = v₀·cos(θ)·t
  currentPos.x = vx0 * currentTime;
  
  // y(t) = h₀ + v₀·sin(θ)·t - 0.5·g·t²
  currentPos.y = h0 + vy0 * currentTime - 0.5f * g * currentTime * currentTime;
  
  // Current velocities (exact)
  vy = vy0 - g * currentTime;
  vx = vx0; // constant
  
  // Track maximum height
  if (currentPos.y > maxHeight) {
    maxHeight = currentPos.y;
  }
  
  // Update trail
  updateTrail();
  
  // Check for ground collision (no bounce)
  if (currentPos.y <= 0) {
    currentPos.y = 0;
    simulationComplete = true;
    
    // Calculate exact range when ball hits ground
    // Solve for t when y=0: h₀ + v₀·sin(θ)·t - 0.5·g·t² = 0
    // Quadratic: a = -0.5*g, b = v₀·sin(θ), c = h₀
    float a = -0.5f * g;
    float b = vy0;
    float c = h0;
    
    float discriminant = b * b - 4.0f * a * c;
    if (discriminant >= 0) {
      float t1 = (-b + sqrt(discriminant)) / (2.0f * a);
      float t2 = (-b - sqrt(discriminant)) / (2.0f * a);
      float totalTime = max(t1, t2);
      
      // Exact range
      totalRange = vx0 * totalTime;
    } else {
      totalRange = currentPos.x;
    }
  }
}

void PhysicsEngine::calculatePrediction() {
  // Calculate trajectory points until ground impact
  predictionPoints = 0;
  
  float angleRad = angle;
  float v0x = v0 * cos(angleRad);
  float v0y = v0 * sin(angleRad);
  
  // Time to hit ground (solve quadratic)
  float a = -0.5f * g;
  float b = v0y;
  float c = h0;
  
  // Discriminant
  float discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    // No real solution (won't hit ground)
    return;
  }
  
  float t1 = (-b + sqrt(discriminant)) / (2 * a);
  float t2 = (-b - sqrt(discriminant)) / (2 * a);
  float totalTime = max(t1, t2);
  
  if (totalTime <= 0) return;
  
  // Generate points
  float step = totalTime / (MAX_PREDICTION_POINTS - 1);
  for (int i = 0; i < MAX_PREDICTION_POINTS; i++) {
    float t = i * step;
    if (t > totalTime) t = totalTime;
    
    prediction[i].x = v0x * t;
    prediction[i].y = h0 + v0y * t - 0.5f * g * t * t;
    
    if (prediction[i].y < 0) {
      prediction[i].y = 0;
      predictionPoints = i + 1;
      break;
    }
    
    predictionPoints++;
  }
}

void PhysicsEngine::updateTrail() {
  // Add current position to trail
  trail[trailIndex] = {currentPos.x, currentPos.y, 0};
  trailIndex = (trailIndex + 1) % MAX_TRAIL_POINTS;
  
  // Update ages
  for (int i = 0; i < MAX_TRAIL_POINTS; i++) {
    if (trail[i].age < 255) {
      trail[i].age += 8; // Faster aging for trail
    }
  }
  
  // Update trail length
  trailLength = min(trailLength + 1, MAX_TRAIL_POINTS);
}

void PhysicsEngine::stopSimulation() {
  simulationComplete = true;
  totalRange = currentPos.x;
  
  // Finalize trail
  for (int i = 0; i < MAX_TRAIL_POINTS; i++) {
    trail[i].age = 255; // Mark as old
  }
}