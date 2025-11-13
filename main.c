#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "pid_controller.h"
#include "valve_simulator.h"

typedef struct {
    PIDController pid;
    ValveSimulator valve;

    // system status
    bool running;
    float current_time;

    //Monitoring
    float position_error;
    float control_effort;
} HydraulicSystem;

int main() 
{
    // 1. Initialize system
    //    - Create HydraulicSystem struct
    //    - Initialize PID with gains (start with Kp=2.0, Ki=0.1, Kd=0.05)
    //    - Initialize valve simulator (time_constant=0.2 seconds)
    //    - Set initial setpoint (e.g., 50%)

    // 2. Print startup message
    //    - Display PID gains
    //    - Display initial setpoint

    // 3. Main control loop (run for 10 seconds of simulation
    //    while (simpulation_time < 10.0) {
    //        a. Read current valve position
    //        b. Compute PID output
    //        c. Send command to valve simpulator
    //        d. Update valve physics (simulate one time step)
    //        e. Print status every 100ms (time, setpoint, position, error, command)
    //     }

    // 4. Print summary statistics
    //    - Final position
    //    - Final error
    //    - Run time
  
    HydraulicSystem system;

    //Open CSV log file
    FILE* log_file = fopen("control_log.csv", "w");
    if (log_file == NULL) {
        printf("Error opening log file!\n");
        return -1;
    }
    fprintf(log_file, "Time,Setpoint,Position,Error,Command\n");

    // 1. Initialize system
   
    // Initialize PID controller
    pid_init(&system.pid, 5.0f, 4.0f, 0.1f, 0.01f);
    pid_set_setpoint(&system.pid, 50.0f); // Target position 50%

    // Initialize valve simulator
    system.valve.position = 0.0f;
    system.valve.velocity = 0.0f;
    system.valve.time_constant = 0.2f; // seconds
    system.valve.deadband = 0.0f; // %
    system.valve.command = 0.0f;

    system.running = true;
    system.current_time = 0.0f;

    // 2. Print startup message
    printf("Hydraulic Valve Control System Starting...\n");
    printf("PID Gains: Kp=%.1f, Ki=%.1f, Kd=%.1f\n", 
            system.pid.kp, system.pid.ki, system.pid.kd);
    printf("Setpoint: %.1f%%\n\n", system.pid.setpoint);
    printf("Time(s)  Setpoint  Position  Error     Command\n");
    printf("-------  --------  --------  --------  --------\n");

    // 3.Main control loop
    float dt = system.pid.sample_time;

    while (system.running && system.current_time < 30.0f) { // Run for 10 seconds
        
        // Add setpoint changes at specific times
        if (system.current_time >= 10.0f && system.current_time < 10.01f) 
        {
            pid_set_setpoint(&system.pid, 75.0f); // Change setpoint to 80% at 5s
            printf("\n>>> Setpoint changed to 75.0%% <<<\n\n");
        }

        if (system.current_time >= 20.0f && system.current_time < 20.01f) 
        {
            pid_set_setpoint(&system.pid, 25.0f); // Change setpoint to 25% at 10s
            printf("\n>>> Setpoint changed to 25.0%% <<<\n\n");
        }
        
        // a. Read current valve 

        // b. Compute PID output
        float control_signal = pid_compute(&system.pid, system.valve.position);

        // ADD THIS DEBUG LINE:
        /*if (system.current_time > 9.0f) {
            printf("DEBUG: integral=%.2f, error=%.2f, raw_output=%.2f\n", 
                    system.pid.integral, 
                    50.0 - system.valve.position, 
                    control_signal);
        }*/

        // Clamp control signal to  valid valve range [0, 100]%
        if (control_signal < 0.0f) control_signal = 0.0f;
        if (control_signal > 100.0f) control_signal = 100.0f;
        

        // c & d. Send command to valve simulator
        valve_update(&system.valve, control_signal, dt);

        // Calculate error for monitoring
        system.position_error = system.pid.setpoint - system.valve.position;
        system.control_effort = control_signal;

        // e. Print status every 500ms
        if (fmod(system.current_time, 0.5f) < 0.01f) 
        {
            printf("%.2f      %.1f     %.1f      %.1f       %.1f\n",
                system.current_time, 
                system.pid.setpoint,
                system.valve.position, 
                system.position_error, 
                system.control_effort);
        }

        fprintf(log_file, "%.2f,%.1f,%.1f,%.1f,%.1f\n",
                system.current_time, 
                system.pid.setpoint,
                system.valve.position, 
                system.position_error, 
                system.control_effort);

        // Increment time
        system.current_time += dt;
    }

    // 4. Print summary statistics
    printf("\n=== Simulation Complete ===\n");
    printf("Final Position: %.1f%%\n", system.valve.position);
    printf("Final Error: %.1f%%\n", system.position_error);
    printf("Run Time: %.2f seconds\n", system.current_time);

    // Close log file
    fclose(log_file);
    printf("\nData logged to control_log.csv\n");

    return 0; 
}