# Real-Time Scheduling Algorithms Simulator

A lightweight, educational C++ simulator for **Rate Monotonic Scheduling (RMS)** and **Earliest Deadline First (EDF)** real-time scheduling algorithms.

This project implements:

- **Static-priority scheduling (RMS)** with Response Time Analysis (RTA)
- **Dynamic-priority scheduling (EDF)**
- Utilization bound checks
- Deadline miss detection
- Simulation of task execution over time
- Comprehensive reporting

---

## Features

| Feature                              | RMS | EDF |
| ------------------------------------ | --- | --- |
| Fixed-priority scheduling            | Yes | No  |
| Dynamic-priority scheduling          | No  | Yes |
| Liu & Layland Utilization Bound Test | Yes | No  |
| EDF Feasibility Test (`U ≤ 1`)       | No  | Yes |
| Response Time Analysis (RTA)         | Yes | No  |
| Deadline Miss Detection              | Yes | Yes |
| Max Observed Response Time Tracking  | Yes | Yes |
| Gantt-style Execution Trace          | Yes | Yes |

---

## Compilation & Usage

### Requirements

- C++11 or later
- Standard compiler (`g++`, `clang++`, etc.)

### Build

```bash
g++ -std=c++11 -o scheduler main.cpp RateMonotonicScheduler.cpp \
EarliestDeadlineFirst.cpp

# Run RMS mode
./scheduler --mode rms

# Run EDF mode
./scheduler --mode edf
```
