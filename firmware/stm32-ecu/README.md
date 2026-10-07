# STM32 ECU Firmware

This folder is the starting point for the base ECU firmware implementation.

## Planned Modules

- state machine
- safety manager
- fault manager
- CAN communication layer
- BMS interface
- motor controller interface
- ADC / sensor acquisition
- telemetry logger

## Build

Use CMake or Makefile depending on the selected toolchain.

```bash
cd firmware/stm32-ecu
make
```
