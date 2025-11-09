#include "pid_controller.h"

// Initialize PID controller with gains and output limits
// Sets all internal variables to zero
void pid_init(PIDController* pid, float kp, float ki, float kd, float sample_time) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = 0.0f;
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_min = -100.0f; // Minimum output (-100%)
    pid->output_max = 100.0f;  // Maximum output (+100%)
    pid->sample_time = sample_time;
}

// Calculate PID control output based on setpoint and current measurement
float pid_compute(PIDController* pid, float measurement) {
    float error = pid->setpoint - measurement;

    // Accumulate integral
    pid->integral += error * pid->sample_time;

    // Anti-windup: Clamp integral term to prevent excessive accumulation
    float max_integral = pid->output_max / pid->ki;  // Prevent integral alone from saturating
    float min_integral = pid->output_min / pid->ki;
    if (pid->integral > max_integral) pid->integral = max_integral;
    if (pid->integral < min_integral) pid->integral = min_integral;

    // Calculate derivative
    float derivative = (error - pid->prev_error) / pid->sample_time;

    // Compute output
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;

    // Store error for next iteration
    pid->prev_error = error;

    // Clamp output to limits
    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;

    return output;
}

//Update PID setpoint to new target value
void pid_set_setpoint(PIDController* pid, float setpoint) {
    pid->setpoint = setpoint;
}