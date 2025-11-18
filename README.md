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
- Disturbance rejection and robustness
- Sensor noise handling

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
- Real-world system modeling
- Performance analysis under uncertainty

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

## Day 2: Multiple Setpoint Tracking

### New Features
- Dynamic setpoint changes during runtime
- CSV data logging for analysis
- Extended simulation time (30 seconds)
- Three different target positions (50% → 75% → 25%)

### Performance Results - Day 2
```
Phase 1 (0-10s):  50% setpoint → Settled at 50.0% 
Phase 2 (10-20s): 75% setpoint → Settled at 75.0%
Phase 3 (20-30s): 25% setpoint → Settled at 25.0%

Rise Time: ~0.2 seconds
Overshoot: None (0%)
Settling Time: ~7 seconds per setpoint
```

### Data Logging
- Full simulation data exported to CSV format
- 3000+ data points captured at 10ms intervals
- Compatible with Excel, Python, MATLAB for analysis

## Day 3: Disturbance Rejection & Robustness Testing

### New Features
- External disturbance simulation (simulates real-world forces on valve)
- Sensor noise injection (±0.1% measurement noise)
- Disturbance rejection analysis
- Real-world operating conditions testing

### Technical Implementation
- Modified valve physics to accept external force inputs
- Implemented measurement noise using random number generation
- Extended simulation to test multiple disturbance scenarios
- Continuous data logging of system response under disturbances

### Disturbance Rejection Performance
```
Test 1: -10% Downward Force (5.0-8.0s)
  Maximum Position Deviation: 1.7% (50.0% → 48.3%)
  PID Compensation: Increased output by 10% (52% → 60%)
  Recovery Time: 2 seconds after disturbance removed
  Steady-State Error with Active Disturbance: ~1%

Test 2: +5% Upward Force (15.0-18.0s)
  Maximum Position Deviation: 1.5% (75.0% → 76.5%)
  PID Compensation: Decreased output by 5%
  Recovery Time: <2 seconds
  Demonstrated bi-directional disturbance handling
```

### Key Capabilities Demonstrated
- **Automatic disturbance compensation** - PID adjusts output without explicit disturbance detection
- **Robust tracking** - Maintains setpoint within 2% despite 10% external forces
- **Noise filtering** - Handles sensor noise without oscillation
- **Real-world applicability** - Simulates friction, pressure variations, mechanical binding

### Performance Metrics Summary
| Metric | Value |
|--------|-------|
| Disturbance Rejection Ratio | 5.9:1 (-10% force → 1.7% error) |
| Recovery Time | <2 seconds |
| Max Steady-State Error (under load) | 1% |
| Sensor Noise Tolerance | ±0.1% |
| Control Signal Range | 0-100% |

