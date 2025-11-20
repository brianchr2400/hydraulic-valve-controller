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

## Day 4: PID Tuning & Performance Optimization

### Overview
Systematic comparison of five different PID tuning strategies to understand gain trade-offs and optimize controller performance. Implemented command-line argument parsing for automated testing and developed rigorous performance analysis methodology.

### New Features
- Command-line parameter input (custom gains via argv)
- Dynamic CSV filename generation based on gains
- Automated testing framework for multiple configurations
- Comprehensive performance metric analysis

### Test Configurations

| Configuration | Kp | Ki | Kd | Strategy |
|---------------|----|----|----|----|
| Baseline | 5.0 | 4.0 | 0.1 | Balanced performance |
| Aggressive | 8.0 | 6.0 | 0.2 | Maximum speed |
| Conservative | 3.0 | 2.0 | 0.05 | Maximum stability |
| High Derivative | 5.0 | 4.0 | 0.5 | Overshoot prevention |
| Low Integral | 5.0 | 1.0 | 0.1 | Minimal integral action |

### Performance Comparison

| Tuning | Rise Time (s) | Overshoot (%) | Settling Time (s) | SS Error (%) |
|--------|---------------|---------------|-------------------|--------------|
| **Baseline** | **0.18** | **0** | **1.75** | **0** |
| Aggressive | 0.16 | 2.0 | 3.37 | 0.6 (osc) |
| Conservative | 1.10 | 0 | 3.57 | 0 |
| High Derivative | 1.44 | 0 | Never | 4.1 (osc) |
| Low Integral | 4.05 | 0 | Never | 1.4 |

### Key Insights

**Baseline Tuning is Optimal**
- Best overall performance across all metrics
- 0% overshoot, 0% steady-state error
- Fastest settling time (1.75s)
- Stable with no oscillations

**Aggressive Tuning Paradox**
- 11% faster rise time but 92% slower settling
- Creates persistent oscillations
- "Fastest" response doesn't mean best performance

**Critical Role of Integral Gain**
- Ki=4.0: Perfect tracking (0% error)
- Ki=1.0: System stuck at 1.4% error permanently
- Integral term is essential for accuracy

**Derivative Gain Limits**
- Kd=0.1: Optimal damping
- Kd=0.5: Over-damped, never reaches target
- Too much derivative is as harmful as too little

### Performance Metrics Definitions

- **Rise Time:** Time to reach 95% of setpoint step (71.25% for 50→75% change)
- **Overshoot:** Maximum excursion beyond setpoint as percentage of step size
- **Settling Time:** Time to enter and remain within ±1% of setpoint
- **Steady-State Error:** Final tracking error after system stabilizes

### Tuning Recommendations

**For Safety-Critical Systems:**
- Use Baseline tuning (Kp=5.0, Ki=4.0, Kd=0.1)
- Zero overshoot, stable response

**For High-Speed Applications:**
- Use Baseline tuning (fastest overall settling despite slightly slower rise)
- Aggressive tuning's oscillations negate its rise time advantage

**For Precision Positioning:**
- Use Baseline tuning (zero steady-state error)
- Ensure Ki ≥ 4.0 for error elimination

### Technical Implementation
- argc/argv command-line parsing
- sprintf() for dynamic filename generation
- Systematic test methodology
- Excel-based data analysis workflow

