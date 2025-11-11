# Hydraulic Valve Controller - PID Control System

## Overview
A simulated hydraulic proportional valve controller with PID position control, demonstrating real-time control systems concepts commonly used in industrial automation.

## Technical Achievements
- Implemented PID control algorithm from scratch in C
- Achieved zero steady-state error (50.0% setpoint tracking)
- 7-second settling time with no overshoot
- First-order lag valve physics simulation
- 10ms control loop execution (100Hz update rate)

## Key Concepts Demonstrated
- Proportional-Integral-Derivative (PID) control
- Control loop timing and sampling
- Anti-windup protection
- System tuning and optimization
- Real-time embedded systems patterns

## Final Tuned Parameters
- Kp = 5.0 (Proportional gain)
- Ki = 4.0 (Integral gain)
- Kd = 0.1 (Derivative gain)
- Sample time = 10ms
- Valve time constant = 0.2s

## Performance Results
```
Settling Time: 7.09 seconds
Steady-State Error: 0.0%
Overshoot: None
Final Position: 50.0% (perfect tracking)
```

## Skills Applied
- C programming
- Control systems theory
- Algorithm implementation
- Debugging and optimization
- Git version control

## Built With
- C (ISO C99)
- GCC compiler
- Windows/MSYS2 development environment

---
*Completed as part of embedded systems portfolio development - Day 1 of 12-week learning plan*

## Performance Results

![Day 1 Perfect Tracking](screenshots/day1_perfect_tracking.png)

Settling Time: 7.09 seconds
Steady-State Error: 0.0%
