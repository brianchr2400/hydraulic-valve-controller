#ifndef VALVE_SIMULATOR_H
#define VALVE_SIMULATOR_H

typedef struct {
    // Current state
    float position; // Current valve position (0-100%)
    float velocity; // Rate of change
    
    //Physical parameters
    float time_constant; // First-order lag time constant (seconds)
    float deadband;     // Deadband zone (% where valve doesn't respond)

    // Command input
    float command; // Control signal from PID (-100 to +100%)
    
    // External disturbance
    float disturbance; // External force a(%))
} ValveSimulator;

void valve_update(ValveSimulator* valve, float command, float dt);
float valve_get_position(ValveSimulator* valve);
void valve_init(ValveSimulator* valve, float time_constant, float deadband);
#endif // VALVE_SIMULATOR_H