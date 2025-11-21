#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pid_controller.h"
#include "valve_simulator.h"

typedef struct {
    PIDController pid;
    ValveSimulator valve;
    bool running;
    float current_time;
    float position_error;
    float control_effort;
} HydraulicSystem;

int main() {
    HydraulicSystem system;
    
    FILE* log_file = fopen("setpoint_ramping_test.csv", "w");
    if (log_file == NULL) {
        printf("Error opening log file!\n");
        return -1;
    }
    fprintf(log_file, "Time,Setpoint,Position,Error,Command\n");
    
    // Initialize PID
    pid_init(&system.pid, 5.0f, 4.0f, 0.1f, 0.01f);
    
    // Enable advanced features
    pid_set_derivative_filter(&system.pid, 0.1f);
    pid_set_rate_limit(&system.pid, 100.0f);
    
    // NEW: Enable setpoint ramping at 10%/second
    pid_set_ramp_rate(&system.pid, 10.0f);  // Ramp at 10%/second
    
    pid_set_setpoint(&system.pid, 50.0f);
    
    // Initialize valve
    system.valve.position = 0.0f;
    system.valve.velocity = 0.0f;
    system.valve.time_constant = 0.2f;
    system.valve.deadband = 0.0f;
    system.valve.command = 0.0f;
    system.valve.disturbance = 0.0f;
    
    system.running = true;
    system.current_time = 0.0f;
    
    printf("Testing Setpoint Ramping\n");
    printf("- Derivative filtering: 0.1\n");
    printf("- Rate limiting: 100%%/second\n");
    printf("- Setpoint ramp rate: 10%%/second\n\n");
    printf("Time(s)  Setpoint  Position  Error     Command\n");
    printf("-------  --------  --------  --------  --------\n");
    
    float dt = system.pid.sample_time;
    
    while (system.running && system.current_time < 10.0f) {
        
        // Setpoint change at 5 seconds
        if (system.current_time >= 5.0f && system.current_time < 5.01f) {
            pid_set_setpoint_ramped(&system.pid, 75.0f);
            printf("\n>>> Setpoint ramp to 75.0%% started (10%%/sec) <<<\n\n");
        }
        
        // Compute control signal
        float control_signal = pid_compute(&system.pid, system.valve.position);
        
        // Clamp to valve range
        if (control_signal < 0.0f) control_signal = 0.0f;
        if (control_signal > 100.0f) control_signal = 100.0f;
        
        // Update valve
        valve_update(&system.valve, control_signal, dt);
        
        // Calculate monitoring values
        system.position_error = system.pid.setpoint - system.valve.position;
        system.control_effort = control_signal;
        
        // Print every 0.5 seconds
        if (fmod(system.current_time, 0.5f) < 0.01f) {
            printf("%.2f      %.1f     %.1f      %.1f       %.1f\n",
                system.current_time, 
                system.pid.setpoint,
                system.valve.position, 
                system.position_error, 
                system.control_effort);
        }
        
        // Log every timestep
        fprintf(log_file, "%.2f,%.1f,%.1f,%.1f,%.1f\n",
                system.current_time, 
                system.pid.setpoint,
                system.valve.position, 
                system.position_error, 
                system.control_effort);
        
        system.current_time += dt;
    }
    
    printf("\n=== Test Complete ===\n");
    printf("Final Position: %.1f%%\n", system.valve.position);
    printf("Final Error: %.1f%%\n", system.position_error);
    
    fclose(log_file);
    printf("\nData logged to setpoint_ramping_test.csv\n");
    
    return 0;
}