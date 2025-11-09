
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {
    //PID gains
    float kp; // Proportional gain
    float ki; // Integral gain
    float kd; // Derivative gain

    // Control variables
    float setpoint;      // Desired position (0-100%)
    float integral;     // Accumulated integral term
    float prev_error; // Previous error for derivative

    // Output limits
    float output_min;   // Minimum output (-100%)
    float output_max;   // Maximum output (+100%)

    // Timing
    float sample_time; // Loop time in seconds (e.g., 0.01 = 10ms)
} PIDController;

// Function prototypes
void pid_init(PIDController* pid, float kp, float ki, float kd, float sample_time);
float pid_compute(PIDController* pid, float measurement);
void pid_set_setpoint(PIDController* pid, float setpoint);

#endif // PID_CONTROLLER_H

