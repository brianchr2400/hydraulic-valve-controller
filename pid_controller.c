#include "pid_controller.h"

// Initialize PID controller with gains and output limits
// Sets all internal variables to zero
void pid_init(PIDController* pid, float kp, float ki, float kd, float sample_time) 
    {
        pid->kp = kp;
        pid->ki = ki;
        pid->kd = kd;
        pid->setpoint = 0.0f;
        pid->integral = 0.0f;
        pid->prev_error = 0.0f;
        pid->output_min = -100.0f; // Minimum output (-100%)
        pid->output_max = 100.0f;  // Maximum output (+100%)
        pid->sample_time = sample_time;

        // Initialize advanced features
        pid->derivative_filtered = 0.0f;
        pid->derivative_filter_coeff = 0.1f; // Default moderate filtering
        pid->prev_output = 0.0f;
        pid->max_rate_of_change = 0.0f; // Disabled by default

        // Initialize ramping
        pid->setpoint_target = 0.0f;
        pid->setpoint_ramp_rate = 0.0f; // Disabled by default
    }

    // Configure derivative filtering
    // filter_coeff: 0 = no filtering; 1 = max filtering
    // Typical value: 0.1 (10% new, 90% old)
void pid_set_derivative_filter(PIDController* pid, float filter_coeff) 
    {
        if (filter_coeff < 0.0f) filter_coeff = 0.0f;
        if (filter_coeff > 1.0f) filter_coeff = 1.0f;
        pid->derivative_filter_coeff = filter_coeff;
    }

// Configure output rate limiting
// max_rate: Maximum change in output per second (0 = disabled)
void pid_set_rate_limit(PIDController* pid, float max_rate) 
    {
        pid->max_rate_of_change = max_rate;
    }

// Calculate PID control output based on setpoint and current measurement
float pid_compute(PIDController* pid, float measurement) 
    {
        if (pid->setpoint_ramp_rate > 0.0f) 
            {
                // Apply setpoint ramping
                float max_change = pid->setpoint_ramp_rate * pid->sample_time;
                float setpoint_error = pid->setpoint_target - pid->setpoint;

                if (setpoint_error > max_change) 
                    {
                        pid->setpoint += max_change;
                    } 
                else if (setpoint_error < -max_change) 
                    {
                        pid->setpoint -= max_change;
                    } 
                else 
                    {
                        pid->setpoint = pid->setpoint_target; // Reached target
                    }
            }
        float error = pid->setpoint - measurement;

        // Proportional term
        float p_term = pid->kp * error;

        // Accumulate integral
        pid->integral += error * pid->sample_time;

        // Anti-windup: Clamp integral term to prevent excessive accumulation
        float max_integral = pid->output_max / pid->ki;  // Prevent integral alone from saturating
        float min_integral = pid->output_min / pid->ki;
        if (pid->integral > max_integral) pid->integral = max_integral;
        if (pid->integral < min_integral) pid->integral = min_integral;

        float i_term = pid->ki * pid->integral;

        // Calculate derivative
        float derivative_raw = (error - pid->prev_error) / pid->sample_time;

        // Apply low-pass filter to derivative
        pid->derivative_filtered = (pid->derivative_filter_coeff * derivative_raw) +
                                   ((1.0f - pid->derivative_filter_coeff) * pid->derivative_filtered);

        float d_term = pid->kd * pid->derivative_filtered;

        // Compute raw output
        float output = p_term + i_term + d_term;

        // Apply rate limiting if enabled
        if (pid->max_rate_of_change > 0.0f) 
            {
                float max_change = pid->max_rate_of_change * pid->sample_time;
                float output_change = output - pid->prev_output;

                if (output_change > max_change) 
                    {
                        output = pid->prev_output + max_change;
                    } 
                else if (output_change < -max_change) 
                    {
                        output = pid->prev_output - max_change;
                        }
            }

        // Store current output for next iteration
        pid->prev_output = output;

        // Store error for next iteration
        pid->prev_error = error;

        // Clamp output to limits
        if (output > pid->output_max) output = pid->output_max;
        if (output < pid->output_min) output = pid->output_min;

        return output;
    }

//Update PID setpoint to new target value
void pid_set_setpoint(PIDController* pid, float setpoint) 
    {
        pid->setpoint = setpoint;
        pid->setpoint_target = setpoint; // Also update target for ramping
    }

// Configure setpoint ramp rate
//ramp_rate: Maximum setpoint change per second (0 = instant/disabled)
// Example: ramp_rate = 10 means setpoint changes at 10%/second
void pid_set_ramp_rate(PIDController* pid, float ramp_rate) 
    {
        pid->setpoint_ramp_rate = ramp_rate;
    }

// Set target setpoint with ramping
// If ramping is enabled, setpoint will gradually move toward target
void pid_set_setpoint_ramped(PIDController* pid, float target_setpoint) 
    {
        pid->setpoint_target = target_setpoint;

        // If ramping is disabled, jump immediately
        if (pid->setpoint_ramp_rate <= 0.0f) 
            {
                pid->setpoint = target_setpoint;
            }
            // Otherwise, ramping will happen in pid_compute()
    }