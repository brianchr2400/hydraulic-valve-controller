# Getting Started - Hydraulic Valve Controller

## What This Project Does

This is a **PID (Proportional-Integral-Derivative) controller** for a simulated hydraulic proportional valve. Think of it like cruise control in a car, but for industrial equipment:

- **Goal:** Keep valve position exactly at a target (setpoint)
- **Method:** PID algorithm continuously adjusts valve command based on error
- **Result:** Real-time control system demonstrating industrial embedded systems concepts

## Quick Start (5 Minutes)

### 1. Compile the Code
```bash
gcc -o valve_controller main.c pid_controller.c valve_simulator.c -lm
```

### 2. Run the Simulation
```bash
./valve_controller
```

### 3. View Results
The program outputs:
- **Console display:** Real-time control output showing setpoint, position, error
- **CSV file:** `tuning_kp5.0_ki4.0_kd0.1.csv` with 1500 data points

Open the CSV file in **Excel** or your favorite spreadsheet to visualize the control response!

---

## Understanding the Output

### Console Output Example
```
Time(s)  Setpoint  Position  Error     Command
-------  --------  --------  --------  --------
0.00      50.0     0.0       50.0      50.0
0.50      50.0     28.2      21.8      44.2
1.00      50.0     40.3      9.7       37.3
...
5.00      75.0     50.0      25.0      50.0      ← Setpoint changes!
5.50      75.0     64.2      10.8      65.5      ← Controller responds
6.00      75.0     70.9      4.1       75.8
...
15.00     75.0     75.0      0.0       75.0      ← Perfect tracking
```

**What each column means:**
- **Time:** Simulation time in seconds
- **Setpoint:** Target position (0-100%)
- **Position:** Current valve position (what we're trying to control)
- **Error:** Difference between setpoint and position
- **Command:** What the PID controller is sending to the valve (0-100%)

### CSV File (for analysis in Excel/Python)
```
Time,Setpoint,Position,Error,Command
0.00,50.0,0.0,50.0,50.0
0.01,50.0,0.4,49.6,49.6
0.02,50.0,0.7,49.3,49.3
...
```

---

## Key Concepts

### PID Control
The controller adjusts output based on three terms:

1. **Proportional (P):** Fast reaction to error
   - Higher Kp = Faster response
   - Too high = Overshoot and oscillation

2. **Integral (I):** Eliminates steady-state error over time
   - Accumulates error to push system to target
   - Critical for accuracy
   - Too high = Slow, oscillatory response

3. **Derivative (D):** Dampens overshoot
   - Reacts to rate of change
   - Prevents bouncing past target
   - Too high = System becomes sluggish

### Real-World Analogy
Imagine adjusting a shower temperature:
- **P term:** "It's cold, turn up the heat" (immediate reaction)
- **I term:** "Still cold after 5 seconds, turn UP MORE" (persistent action)
- **D term:** "Whoa, getting HOT, back off!" (anticipation)

Together = comfortable warm shower without oscillating between cold and hot!

---

## Project Files

```
.
├── main.c                    # Main program orchestrating the system
├── pid_controller.c/h        # PID algorithm implementation
├── valve_simulator.c/h       # Valve physics simulation (first-order lag)
├── README.md                 # Project documentation
├── LEARNING_LOG.md           # 5-day learning progression
├── BUILD.md                  # Detailed build instructions
├── GETTING_STARTED.md        # This file
├── PROJECT_STRUCTURE.md      # Architecture explanation
└── tuning_kp*.csv            # Generated data files
```

---

## Running Different Tests

### Default (Baseline - Best Overall)
```bash
./valve_controller
```
- Kp=5.0, Ki=4.0, Kd=0.1
- Perfect 0% overshoot, fastest settling

### Aggressive (Fast but Oscillates)
```bash
./valve_controller 8.0 6.0 0.2
```
- Kp=8.0, Ki=6.0, Kd=0.2
- Faster rise time but creates oscillations

### Conservative (Slow but Very Stable)
```bash
./valve_controller 3.0 2.0 0.05
```
- Kp=3.0, Ki=2.0, Kd=0.05
- Super stable, slower response

### High Derivative (Over-damped)
```bash
./valve_controller 5.0 4.0 0.5
```
- Kp=5.0, Ki=4.0, Kd=0.5
- Too much damping, never reaches target

### Low Integral (Steady-State Error)
```bash
./valve_controller 5.0 1.0 0.1
```
- Kp=5.0, Ki=1.0, Kd=0.1
- Insufficient integral, permanent 1.4% error

Compare the CSV outputs from each test to see how gains affect behavior!

---

## Analysis in Excel

### Import CSV Data
1. Open Excel
2. File → Open → Select `tuning_kp*.csv`
3. Data is auto-imported

### Create a Chart
1. Select columns: Time, Position, Setpoint
2. Insert → Line Chart
3. You'll see how position tracks setpoint over time

### Analyze Performance
Look for:
- **Rise Time:** How fast does position reach 71.25% after setpoint changes? (ideally ~0.2s)
- **Overshoot:** Does position exceed setpoint? (ideally 0%)
- **Settling Time:** When does position stay within ±1% of setpoint? (ideally 1-2 seconds)
- **Steady-State Error:** Final error when settled? (ideally 0%)

---

## Analysis in Python

```python
import pandas as pd
import matplotlib.pyplot as plt

# Load data
df = pd.read_csv('tuning_kp5.0_ki4.0_kd0.1.csv')

# Plot
plt.figure(figsize=(12, 6))
plt.plot(df['Time'], df['Setpoint'], label='Setpoint', linestyle='--')
plt.plot(df['Time'], df['Position'], label='Position')
plt.xlabel('Time (seconds)')
plt.ylabel('Position (%)')
plt.title('PID Controller Response')
plt.legend()
plt.grid(True)
plt.show()
```

---

## Next Steps

### Learn More
1. Read **LEARNING_LOG.md** to see the 5-day learning journey
2. Review **README.md** for technical details
3. Check **PROJECT_STRUCTURE.md** for code architecture
4. Read comments in **pid_controller.c** to understand the algorithm

### Experiment
1. Try different Kp, Ki, Kd values
2. Compare CSV outputs to see how gains affect response
3. Try to find YOUR OWN optimal tuning!

### Extend the Project
- Add different setpoint trajectories (ramps, sine waves)
- Implement different control algorithms (state-space, sliding mode)
- Add physical constraints (rate limiting, saturation)
- Integrate with real hardware (Arduino, STM32)

---

## Troubleshooting

**Q: Program doesn't compile**
→ See BUILD.md section "Troubleshooting"

**Q: No CSV file created**
→ Check permissions in current directory
→ Filename is `tuning_kp5.0_ki4.0_kd0.1.csv` for default run

**Q: Position doesn't reach setpoint**
→ This is normal for Week 1 experiments!
→ Try running: `./valve_controller 5.0 4.0 0.1` (baseline)
→ Baseline achieves perfect 50.0% or 75.0% tracking

**Q: What does "Integral" mean in the output?**
→ It's the accumulated error term
→ Growing integral value = controller adding more "push"
→ When position stabilizes, integral stays constant

---

## File Descriptions

### main.c
- Orchestrates the control system
- Initializes PID and valve simulator
- Runs the main control loop
- Handles CSV logging

### pid_controller.c/h
- Pure PID algorithm implementation
- Includes anti-windup protection
- Advanced features: derivative filtering, rate limiting, setpoint ramping
- Can be reused in other projects

### valve_simulator.c/h
- Simulates physical valve dynamics
- First-order lag model (realistic)
- Supports external disturbances
- Can be replaced with real hardware interface

### README.md
- Project overview
- Technical achievements
- Performance results and data

### LEARNING_LOG.md
- Detailed 5-day learning progression
- What was learned each day
- Challenges overcome
- Key takeaways

---

Enjoy exploring PID control! 🎯