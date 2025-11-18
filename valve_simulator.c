#include "valve_simulator.h"

// Initialize valve simulator with physical parameters
// Sets initial position to 0%, velocity to 0
void valve_init(ValveSimulator* valve, float time_constant, float deadband) {
    valve->position = 0.0f;
    valve->velocity = 0.0f;
    valve->time_constant = time_constant;
    valve->deadband = deadband;
    valve->command = 0.0f;
}

//Update valve physics for one time step using first-order lag model
// Input: control command (-100 to +100%)
// Update position and velocity based on time constant
void valve_update(ValveSimulator* valve, float command, float dt) {
    valve->command = command;

    // Apply disturbance to the effective command
    float effective_command = valve->command + valve->disturbance;

    // First-order lag: position moves toward command exponentially
    float position_change = (effective_command - valve->position) * (dt / valve->time_constant);
    
    // Update position
    valve->position += position_change;

    // Clamp position to [0, 100]%
    if (valve->position < 0.0f) valve->position = 0.0f;
    if (valve->position > 100.0f) valve->position = 100.0f;

    // Calculate velocity for monitoring purposes
    valve->velocity = position_change / dt;
}

// Get current valve position (0-100%)
float valve_get_position(ValveSimulator* valve) {
    return valve->position;
}
