
#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {
    //PID gains
    float kp; // Proportional gain
    float ki; // Integral gain
    float kd; // Derivative gain

    // Control variables
    float setpoint;     // Desired position (0-100%)
    float integral;     // Accumulated integral term
    float prev_error;   // Previous error for derivative

    // Output limits
    float output_min;   // Minimum output (-100%)
    float output_max;   // Maximum output (+100%)

    // Timing
    float sample_time;  // Loop time in seconds (e.g., 0.01 = 10ms)

    // Advanced features
    float derivative_filtered;     // Filtered derivative term
    float derivative_filter_coeff; // Filter coefficient (0-1, typical: 0.1)
    float prev_output;             // Previous output for rate limiting
    float max_rate_of_change;      // Maximum output change per second (0 = disabled)

    // Setpoint ramping
    float setpoint_target;   // Target setpoint (where we want to go)
    float setpoint_ramp_rate; // Ramp rate (%/second, 0 =  instant/disabled)
} PIDController;

// Function prototypes
void pid_init(PIDController* pid, float kp, float ki, float kd, float sample_time);
float pid_compute(PIDController* pid, float measurement);
void pid_set_setpoint(PIDController* pid, float setpoint);

//Advanced configuration
void pid_set_derivative_filter(PIDController* pid, float filter_coeff);
void pid_set_rate_limit(PIDController* pid, float max_rate);

// Setpoint ramping
void pid_set_setpoint_ramped(PIDController* pid, float target_setpoint);
void pid_set_ramp_rate(PIDController* pid, float ramp_rate);

#endif // PID_CONTROLLER_H

