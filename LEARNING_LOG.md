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

## Day 2 - November 13, 2025

### What I Built
Extended PID controller with multiple setpoint tracking and CSV data logging

### What I Learned
1. **Dynamic Setpoint Changes:**
   - Controller automatically tracks new targets
   - Integral term needs time to fully eliminate error
   - With sufficient settling time (10s), achieved perfect tracking

2. **Data Logging:**
   - File I/O in C (fopen, fprintf, fclose)
   - CSV format for data export
   - Console output vs data logging separation

3. **Performance Analysis:**
   - Rise time: ~0.17-0.2 seconds
   - No overshoot with current gains
   - Settling time depends on Ki gain and available time

### Challenges Overcome
- Initial 5-second settling was too short
- Extended to 10 seconds per setpoint for full settling
- Implemented CSV logging for long simulations
- Reduced console spam by printing every 0.5s instead of 0.01s

### Key Takeaway
The integral term needs adequate time to eliminate steady-state error. With Ki=4.0, approximately 7 seconds is needed for full settling. Data logging is essential for analyzing long simulations.

### Time Spent
~2 hours (including CSV implementation)

### Next Steps
Day 3: Add disturbance rejection and test robustness

