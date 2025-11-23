#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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

int main(int argc, char* argv[]) 
{
    HydraulicSystem system;

    // Default gains
    float kp = 5.0f;
    float ki = 4.0f;
    float kd = 0.1f;

    // Override gains if provided as command-line arguments
    if (argc >= 4) 
        {
            kp = atof(argv[1]);
            ki = atof(argv[2]);
            kd = atof(argv[3]);
            printf("Using custom gains: kp=%.2f, ki=%.2f, kd=%.2f\n", kp, ki, kd);
        }   
    else
        {
            printf("Using default gains: kp=%.2f, ki=%.2f, kd=%.2f\n", kp, ki, kd);
        }

    // Open CSV log file with unique name based on gains
    char filename[100];
    sprintf(filename, "tuning_kp%.1f_ki%.1f_kd%.1f.csv", kp, ki, kd);
    FILE* log_file = fopen(filename, "w");
    if (log_file == NULL) 
        {
            printf("Error opening log file!\n");
            return -1;
        }
    srand(time(NULL)); // Seed random number generator
    fprintf(log_file, "Time,Setpoint,Position,Error,Command\n");

    // Initialize PID controller
    pid_init(&system.pid, kp, ki, kd, 0.01f);
    pid_set_setpoint(&system.pid, 50.0f); // Initial target position 50%

    // Initialize valve simulator
    system.valve.position = 0.0f;
    system.valve.velocity = 0.0f;
    system.valve.time_constant = 0.2f; // seconds
    system.valve.deadband = 0.0f;
    system.valve.command = 0.0f;
    system.valve.disturbance = 0.0f; // No external disturbances

    system.running = true;
    system.current_time = 0.0f;

    // Print startup message
    printf("Hydraulic Valve Control System Starting...\n");
    printf("PID Gains: Kp=%.1f, Ki=%.1f, Kd=%.1f\n", 
            system.pid.kp, system.pid.ki, system.pid.kd);
    printf("Setpoint: %.1f%%\n\n", system.pid.setpoint);
    printf("Time(s)  Setpoint  Position  Error     Command\n");
    printf("-------  --------  --------  --------  --------\n");

    // Main control loop
    float dt = system.pid.sample_time;

    while (system.running && system.current_time < 15.0f) {
        
        // Change setpoint at 5 seconds (50% -> 75%)
        if (system.current_time >= 5.0f && system.current_time < 5.01f) 
            {
                pid_set_setpoint(&system.pid, 75.0f);
                printf("\n>>> Setpoint changed to 75.0%% <<<\n\n");
            }

        // Compute PID output based on current valve position
        float control_signal = pid_compute(&system.pid, system.valve.position);

        // Clamp control signal to valid valve range [0, 100]%
        if (control_signal < 0.0f) control_signal = 0.0f;
        if (control_signal > 100.0f) control_signal = 100.0f;

        // Update valve physics with control signal
        valve_update(&system.valve, control_signal, dt);

        // Calculate error for monitoring
        system.position_error = system.pid.setpoint - system.valve.position;
        system.control_effort = control_signal;

        // Print status every 500ms
        if (fmod(system.current_time, 0.5f) < 0.01f) 
            {
                printf("%.2f      %.1f     %.1f      %.1f       %.1f\n",
                    system.current_time, 
                    system.pid.setpoint,
                    system.valve.position, 
                    system.position_error, 
                    system.control_effort);
            }

        // Log all data to CSV
        fprintf(log_file, "%.2f,%.1f,%.1f,%.1f,%.1f\n",
                system.current_time, 
                system.pid.setpoint,
                system.valve.position, 
                system.position_error, 
                system.control_effort);

        // Increment time
        system.current_time += dt;
    }

    // Print summary statistics
    printf("\n=== Simulation Complete ===\n");
    printf("Final Position: %.1f%%\n", system.valve.position);
    printf("Final Error: %.1f%%\n", system.position_error);
    printf("Run Time: %.2f seconds\n", system.current_time);

    // Close log file
    fclose(log_file);
    printf("\nData logged to %s\n", filename);

    return 0; 
}