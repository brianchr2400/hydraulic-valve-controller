# Learning Log

# Day 1 - November 9, 2025

## What I Built
Hydraulic valve PID controller simulator

## What I Learned
1. **PID Control Theory:**
   - P term: Fast response (Kp=5.0)
   - I term: Eliminates steady-state error (Ki=4.0)
   - D term: Prevents overshoot (Kd=0.1)

2. **Tuning Process:**
   - Started with Ki=2.0, position stuck at 49.7%
   - Increased Ki to 4.0, achieved perfect 50.0%
   - Learned: Higher Ki = faster error elimination

3. **C Programming:**
   - Struct-based design
   - Function pointers and modular code
   - Fixed typedef syntax error

## Challenges Overcome
- Set up GCC on Windows (MSYS2)
- Fixed VS Code IntelliSense configuration
- Debugged steady-state error issue
- Tuned PID gains for optimal performance

## Key Takeaway
The integral term is critical for eliminating steady-state error in control systems. Without sufficient Ki gain, the system will never quite reach the setpoint.

## Time Spent
~3-4 hours (including environment setup)

## Next Steps
Day 2: Add setpoint changes and disturbance rejection
