# Safety

This document defines the operational safety model for the prototype.

## Design Priority

Safety > reliability > simplicity > cost > performance

## Mandatory Safety Features

- emergency stop button
- main fuse and service disconnect
- DC-rated contactor
- pre-charge circuit
- commercial BMS
- brake switch
- mechanically independent brake system
- closed test area
- fire extinguisher
- insulated tools

## Safety Constraints

- software must never be the only method of de-energizing the HV system
- emergency-stop logic must open power independently of the MCU
- throttle and brake states must be checked for plausibility
- watchdog and timeout behavior must disable torque when communications are lost
- no single point of failure should be allowed to leave the system in an unsafe state

## Fault Handling

When a critical fault is detected:

1. torque request is set to zero
2. contactors are commanded open
3. vehicle health state transitions to fault or shutdown
4. system logs fault details and heartbeat losses

## Operational Notes

This is a prototype and should be treated as a lab or closed-track vehicle only. It is not a road-legal machine.
