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

    // Default goins
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
            printf("Using default gains: ki=%.2f, ki=%.2f, kd=%.2f\n", kp, ki, kd);
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

    // 1. Initialize system
   
    // Initialize PID controller
    pid_init(&system.pid, kp, ki, kd, 0.01f);
    pid_set_setpoint(&system.pid, 50.0f); // Target position 50%

    // Initialize valve simulator
    system.valve.position = 0.0f;
    system.valve.velocity = 0.0f;
    system.valve.time_constant = 0.2f; // seconds
    system.valve.deadband = 0.0f; // %
    system.valve.command = 0.0f;
    system.valve.disturbance = 0.0f; // No disturbance initially

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

    while (system.running && system.current_time < 15.0f) { // Run for 10 seconds
        
        // Apply disturbance at 5 seconds (sudden force)
        //if (system.current_time >= 5.0f && system.current_time < 5.01f) 
        //{
        //    system.valve.disturbance = -10.0f; // S-10% push down
        //    printf("\n>>> DISTURBANCE: -10%% force applied <<<\n\n");
        //}

        // Remove disturbance at 8 seconds
        //if (system.current_time >= 8.0f && system.current_time < 8.01f) 
        //{
        //    system.valve.disturbance = 0.0f; // Remove disturbance
        //    printf("\n>>> DISTURBANCE: Removed <<<\n\n");
        //}

        // Add another disturbance at 15 seconds
        //if (system.current_time >= 15.0f && system.current_time < 15.01f) {
        //    system.valve.disturbance = 5.0f;  // +5% push up
        //    printf("\n>>> DISTURBANCE: +5%% force applied <<<\n\n");
        //}
        
        // Remove at 18 seconds
        //if (system.current_time >= 18.0f && system.current_time < 18.01f) {
        //    system.valve.disturbance = 0.0f;
        //    printf("\n>>> DISTURBANCE: Removed <<<\n\n");
        //}

        // Add setpoint changes at specific times
        if (system.current_time >= 5.0f && system.current_time < 5.01f) 
            {
                pid_set_setpoint(&system.pid, 75.0f); // Change setpoint to 75% at 10s
                printf("\n>>> Setpoint changed to 75.0%% <<<\n\n");
            }

        //if (system.current_time >= 20.0f && system.current_time < 20.01f) 
        //{
        //    pid_set_setpoint(&system.pid, 25.0f); // Change setpoint to 25% at 10s
        //    printf("\n>>> Setpoint changed to 25.0%% <<<\n\n");
        //}
        
        // a. Read current valve 
        // After reading valve position, add small random noise
        // b. Compute PID output
        
        //float noise = ((float)rand() / RAND_MAX - 0.5f) * 0.2f; // +/-0.1%
        float noisy_measurement = system.valve.position;// + noise;
        float control_signal = pid_compute(&system.pid, noisy_measurement);

        
        // ADD THIS DEBUG LINE:
        /*if (system.current_time > 9.0f) {
            printf("DEBUG: integral=%.2f, error=%.2f, raw_output=%.2f\n", 
                    system.pid.integral, 
                    50.0 - system.valve.position, 
                    control_signal);
        }*/

        

        // Use nois

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