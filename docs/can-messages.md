# CAN Message Architecture

## Targets

- CAN 2.0B
- 500 kbit/s
- 120 ohm termination at both ends
- twisted pair wiring
- short stubs

## Representative Message Set

| CAN ID | Sender | Purpose | Rate |
| --- | --- | --- | --- |
| 0x010 | ECU | heartbeat / fault state | 100 Hz |
| 0x100 | BMS | pack voltage/current/SOC | 10 Hz |
| 0x101 | BMS | cell extrema and temperatures | 10 Hz |
| 0x102 | BMS | faults and current limits | 10 Hz |
| 0x200 | ECU | torque request and mode | 100 Hz |
| 0x300 | Motor Controller | RPM and currents | 50 Hz |
| 0x301 | Motor Controller | temperature and voltage | 10 Hz |
| 0x210 | ECU | speed, throttle, brake, and state | 20 Hz |
| 0x211 | ECU | faults and trip data | 10 Hz |
| 0x7E0/0x7E8 | Tester/ECU | diagnostics | on demand |

## Safety policy

- telemetry should be listen-only
- missing BMS data > 500 ms should force a fault
- missing motor controller data > 200 ms should zero torque
- command messages should use CRC and rolling counters where possible
