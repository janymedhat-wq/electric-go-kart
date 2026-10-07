# Architecture

This document captures the system-level architecture for the electric go-kart prototype.

## Purpose

The vehicle is meant to be a single-seat EV platform designed around a conservative, safety-first embedded architecture. The system is intended for private or closed-track use and should be treated as an engineering prototype rather than a road-legal product.

## System Overview

The vehicle combines:

- mechanical chassis and steering system
- battery system and BMS
- HV power distribution and pre-charge
- BLDC motor and controller
- STM32-based ECU
- CAN communication backbone
- telemetry and logging
- safety interlocks and emergency shutdown

## Key Design Baseline

- Vehicle: single-seat electric go-kart
- Battery: 16S LiFePO4, nominal 51.2 V
- Energy: approximately 1.54 kWh
- Motor: 48 V-class BLDC
- Speed target: 40 km/h software-limited
- Primary communications: CAN 2.0B at 500 kbit/s
- Control MCU: STM32

## Functional Hierarchy

```text
Vehicle
├── Mechanical system
├── Powertrain
├── Battery system
├── High-voltage power
├── Low-voltage system
├── Embedded control
├── Communication
└── Data and telemetry
```

## Safety Principles

- Mechanical brake remains independent of software
- Regenerative braking is secondary to braking safety
- Interlock and e-stop circuitry should act independently of software control
- All assumptions in this project are design targets and must be verified before operation

## TODO

- Expand this document with a full module list and interfaces
- Add sequence diagrams for startup, shutdown, and fault handling
- Document battery protections and fault propagation paths
