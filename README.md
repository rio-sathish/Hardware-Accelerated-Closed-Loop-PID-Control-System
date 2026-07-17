# Hardware-Accelerated Closed-Loop PID Control System

A hardware/software co-design project implementing a **Closed-Loop PID Controller** on the **ZedBoard (Xilinx Zynq-7000 XC7Z020)**. The PID computation is accelerated using a custom **AXI4-Lite hardware IP**, while the control software is developed in **Vitis**.

---

## Project Overview

This project demonstrates hardware acceleration of a Proportional–Integral–Derivative (PID) controller using FPGA technology. A custom AXI4-Lite peripheral performs the PID computation, while the embedded software running on the processor configures the controller parameters and communicates with the hardware.

---

## Features

- Hardware-accelerated PID controller
- Custom AXI4-Lite IP core
- Real-time closed-loop control
- Embedded software developed in Vitis
- Modular RTL design in Verilog
- ZedBoard implementation

---

## Hardware

- ZedBoard (XC7Z020)
- Xilinx Vivado 2025.1
- Xilinx Vitis 2025.1

---

## Repository Structure

```
Hardware-Accelerated-Closed-Loop-PID-Control-System/
│
├── docs/
│   ├── Project_Report.pdf
│   └── README.md
│
├── hardware/
│   ├── rtl/
│   │   ├── my_pid_controller.v
│   │   └── my_pid_controller_slave_lite_v1_0_S00_AXI.v
│   │
│   ├── constraints/
│   │   └── zedboard.xdc
│   │
│   └── block_design/
│       ├── design_1.png
│       └── README.md
│
├── vitis/
│   ├── src/
│   │   └── main.c
│   └── README.md
│
└── images/
```

---

## Vivado Block Design

![Vivado Block Design](hardware/block_design/design_1.png)

---

## Hardware Design

The hardware subsystem consists of:

- Custom AXI4-Lite PID IP
- AXI Interconnect
- Processor subsystem
- Clock and Reset logic
- GPIO interfaces
- Memory-mapped registers for PID parameters

---

## Software

The Vitis application:

- Initializes the hardware platform
- Configures PID gains
- Writes parameters through AXI4-Lite
- Reads the computed control effort
- Implements the closed-loop control algorithm

---

## Technologies Used

- Verilog HDL
- Xilinx Vivado
- Xilinx Vitis
- AXI4-Lite Protocol
- Embedded C
- FPGA Hardware Acceleration

---

## Documentation

The complete project report is available in:

```
docs/Project_Report.pdf
```

---

## Future Improvements

- Floating-point PID implementation
- AXI Stream interface
- DMA support
- Real-time sensor integration
- Hardware optimization for lower latency

---

## Author

**Sathish M**

Electronics and Communication Engineering  
PSG College of Technology

GitHub: https://github.com/rio-sathish
