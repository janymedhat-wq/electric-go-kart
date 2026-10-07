# Requirements

## Functional Requirements

- Read throttle and brake signals
- Validate dual-channel throttle plausibility
- Manage state transitions: OFF, INIT, PRECHARGE, READY, DRIVE, FAULT, SHUTDOWN
- Monitor BMS and motor controller status
- Control contactor and pre-charge sequencing
- Enforce speed and torque limits
- Log faults and telemetry data

## Performance Requirements

- ADC filtering at approximately 1 kHz
- Main control loop at 100 Hz
- CAN transmit cadence around 100 Hz for control and heartbeat messages
- BMS supervision at 10 Hz
- Telemetry at 10 Hz

## Safety Requirements

- brake override must cut torque when brake is pressed and throttle is active
- missing BMS data must produce a fault state
- missing motor controller data must reduce or zero torque
- faults must be latched until reset or service action

## Validation

These requirements must be validated using:

- unit tests for state logic and fault handling
- simulation of CAN timeout behavior
- hardware-in-the-loop checks for sensor tolerance and plausibility
- bench and track testing before operation
