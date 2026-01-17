/**
 * Configuration constants for ProjectileMachine_OLED
 */

#ifndef CONFIG_H
#define CONFIG_H

// Compatibility macros
#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef constrain
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

// Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// Physics constants
#define EARTH_GRAVITY 9.81f
#define MOON_GRAVITY 1.62f
#define MIN_GRAVITY 0.1f
#define MAX_GRAVITY 20.0f
#define COEFFICIENT_OF_RESTITUTION 0.0f  // No bounce
#define HORIZONTAL_FRICTION 1.0f  // No friction
#define MAX_BOUNCES 0  // No bounce

// Simulation parameters
#define MIN_HEIGHT 0.0f
#define MAX_HEIGHT 50.0f
#define HEIGHT_STEP 0.5f

#define MIN_ANGLE 0.0f
#define MAX_ANGLE 90.0f
#define ANGLE_STEP 0.5f

#define MIN_VELOCITY 1.0f
#define MAX_VELOCITY 50.0f
#define VELOCITY_STEP 0.5f

#define SIMULATION_DT 0.033f  // ~30 FPS

// Performance settings
#define MAX_PREDICTION_POINTS 60
#define MAX_TRAIL_POINTS 8
#define MAX_DOTTED_POINTS 60
#define FRAME_TIME_MS 33  // ~30 FPS

// Button timing
#define DEBOUNCE_MS 30
#define HOLD_START_MS 350
#define HOLD_REPEAT_MS 80
#define HOLD_ACCELERATE_MS 1200
#define LONG_PRESS_MS 1000

// Audio settings
#define BEEPER_ENABLED 1
#define BEEP_SHORT 100
#define BEEP_MEDIUM 200
#define BEEP_LONG 500
#define BEEP_ERROR 300
#define FLIGHT_BEEP_INTERVAL 150

// UI constants
#define GROUND_Y 56
#define CANNON_X 10
#define CANNON_Y GROUND_Y
#define CANNON_LENGTH 15
#define BALL_RADIUS 2
#define CANNON_MOUTH_OFFSET 2

// Velocity vector settings
#define VECTOR_SCALE 0.8f  // 1 m/s = 0.8 pixels
#define MAX_VECTOR_LENGTH 20
#define VECTOR_ARROW_SIZE 3

// Animation
#define BOOT_ANIM_DURATION 2500  // 2.5 seconds

#endif