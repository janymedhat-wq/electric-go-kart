# Electric Go-Kart / Small EV

## Master Engineering Design & System Architecture

**Project:** Single-seat Electric Go-Kart Prototype\
**Document:** Engineering Architecture, Design Basis, Software,
Electrical, Safety, Testing & Roadmap\
**Revision:** V1.0\
**Status:** Design basis --- estimates must be verified before
construction or driving

> **Engineering priority:** Safety → Reliability → Simplicity → Cost →
> Performance

> **Important:** This document is an engineering plan, not a
> certification. The numerical values are first-pass estimates from the
> project blueprint. Verify component datasheets, electrical ratings,
> structural calculations/FEA, braking performance, thermal performance
> and all safety functions before spending money or operating the
> vehicle.

------------------------------------------------------------------------

# 1. Project Overview

The project is the design, construction, programming and testing of a
**single-seat electric go-kart** whose electrical and control
architecture is deliberately structured so that it can later scale
toward a small electric vehicle.

The V1 vehicle is intended for **closed-track/private-ground use** and
is not road legal or homologated.

The core engineering idea is to build the architecture once:

-   Mechanical chassis
-   Steering and braking
-   Electric motor and reduction drive
-   Battery and BMS
-   High-voltage power distribution
-   Contactors and pre-charge
-   Motor controller
-   STM32 vehicle ECU
-   CAN communication
-   Throttle and brake sensing
-   Dashboard
-   Data logging
-   Telemetry
-   Safety and fault handling

The V1 should therefore be treated as a **real embedded vehicle
system**, not simply a battery connected to a motor.

------------------------------------------------------------------------

# 2. V1 Design Baseline

  Parameter                                                V1 Design Target
  ---------------------------------- --------------------------------------
  Vehicle                                      Single-seat electric go-kart
  Intended environment                        Closed track / private ground
  Total design mass                            170--190 kg including driver
  Kart mass target                                                  \~90 kg
  Driver payload                                                Up to 90 kg
  Battery                                                       16S LiFePO4
  Battery voltage                                            51.2 V nominal
  Battery capacity                                                    30 Ah
  Battery energy                                                 \~1.54 kWh
  Motor                                                     48 V-class BLDC
  Motor continuous power                                               3 kW
  Peak power target                                                  \~6 kW
  Gear reduction                                                      \~4:1
  Drive                                                      Rear live axle
  V1 software speed limit                                           40 km/h
  Hardware capability target                                      \~50 km/h
  0--30 km/h target                                                   \~5 s
  Required range                                                     ≥20 km
  Predicted range                      \~30--35 km under stated assumptions
  Maximum design grade                             15% start-from-stop case
  Front tyres                                                    10 × 4.5-5
  Rear tyres                                                     11 × 7.1-5
  Loaded rolling radius                                           \~0.135 m
  Ground clearance                                                40--60 mm
  Operating temperature assumption                                 0--45 °C
  V1 chassis                             Welded steel, rigid, no suspension
  Control MCU                                                         STM32
  Vehicle network                                      CAN 2.0B, 500 kbit/s
  Dashboard                                          ESP32/STM32 + TFT/OLED
  Estimated V1 budget                  USD 2,000--4,000 + \~15% contingency

These are **design targets, not guaranteed specifications**.

------------------------------------------------------------------------

# 3. System-Level Architecture

## 3.1 Functional hierarchy

``` text
VEHICLE
│
├── Mechanical System
│   ├── Tubular chassis
│   ├── Steering
│   ├── Brakes
│   ├── Wheels / tyres
│   └── Driver interface
│
├── Powertrain
│   ├── Motor controller / inverter
│   ├── BLDC motor
│   ├── Gear reduction
│   └── Rear live axle
│
├── Battery System
│   ├── LiFePO4 cells
│   ├── BMS
│   ├── Current sensing
│   ├── Temperature sensing
│   └── Protection
│
├── High-Voltage Power
│   ├── Main fuse
│   ├── Service disconnect
│   ├── Pre-charge circuit
│   └── Main contactor
│
├── Low-Voltage System
│   ├── DC/DC converter
│   ├── 12 V fuse distribution
│   ├── ECU
│   ├── Dashboard
│   └── Auxiliary loads
│
├── Embedded Control
│   ├── STM32 ECU
│   ├── Vehicle state machine
│   ├── Safety manager
│   ├── Throttle processing
│   ├── Brake processing
│   └── Watchdog
│
├── Communication
│   ├── CAN bus
│   ├── UART
│   ├── I2C
│   ├── SPI
│   └── GPS
│
└── Data / Telemetry
    ├── SD logging
    ├── GPS
    ├── BLE / Wi-Fi
    └── Optional MQTT gateway
```

------------------------------------------------------------------------

# 4. High-Voltage Power Architecture

## 4.1 Propulsion power path

``` text
BATTERY PACK
     │
     ▼
MAIN FUSE
     │
     ▼
SERVICE DISCONNECT
     │
     ├──────── PRE-CHARGE BRANCH ────────┐
     │                                    │
     ▼                                    ▼
MAIN CONTACTOR                      PRE-CHARGE RESISTOR
     │                                    │
     └───────────────┬────────────────────┘
                     ▼
             MOTOR CONTROLLER
                     │
             3-PHASE OUTPUT
                     │
                     ▼
              BLDC MOTOR
                     │
                     ▼
             GEAR REDUCTION
                     │
                     ▼
                REAR AXLE
                     │
                     ▼
                  WHEELS
```

The battery negative returns through the battery
protection/current-sensing architecture to the controller negative.

## 4.2 V1 electrical domains

1.  **HV propulsion domain**
    -   Battery
    -   Fuse
    -   Contactors
    -   Motor controller
    -   Motor
    -   DC/DC input
2.  **12 V auxiliary domain**
    -   ECU
    -   Dashboard
    -   Fans
    -   Horn
    -   Brake light
    -   Contactor coils
    -   Lighting
3.  **5 V sensor domain**
    -   Throttle sensors
    -   Other analogue sensors
4.  **3.3 V logic domain**
    -   STM32
    -   CAN transceiver logic
    -   Digital interfaces

The design should maintain clear separation between noisy power wiring
and sensitive signal wiring.

------------------------------------------------------------------------

# 5. Battery System

## 5.1 Battery configuration

The V1 battery is specified as:

``` text
16S1P LiFePO4
16 × 3.2 V nominal cells
= 51.2 V nominal

30 Ah capacity

Nominal energy:
51.2 V × 30 Ah
= 1,536 Wh
≈ 1.54 kWh
```

The project blueprint specifies LiFePO4 for V1 because of its
comparatively stable chemistry and cycle-life characteristics.

## 5.2 Voltage range

  Condition                                   Approx. voltage
  ---------------------------------- ------------------------
  Nominal                                              51.2 V
  Full-cell maximum at 3.65 V/cell                     58.4 V
  Suggested charge target                            \~57.6 V
  BMS undervoltage cutoff basis        \~44.8 V at 2.8 V/cell
  Absolute cell minimum reference          40 V at 2.5 V/cell

The exact operating limits must be taken from the selected cell and BMS
datasheets.

## 5.3 Battery objectives

The battery pack must provide:

-   Adequate continuous current
-   Adequate short-duration peak current
-   Cell voltage monitoring
-   Temperature monitoring
-   Overcurrent protection
-   Overvoltage protection
-   Undervoltage protection
-   Passive balancing
-   Controlled contactor permission
-   CAN/UART communication
-   Safe physical containment
-   Mechanical retention

The battery should be mounted **low and near the vehicle centreline**.

------------------------------------------------------------------------

# 6. Battery Management System

## 6.1 V1 BMS strategy

V1 should use a **commercial 16S LiFePO4 BMS** rather than a custom BMS.

Target capabilities:

-   ≥100 A continuous discharge capability
-   Higher short-duration peak capability where supported
-   Cell voltage monitoring
-   Multiple temperature inputs
-   Passive balancing
-   CAN or UART
-   Hardware protection
-   Contactor / cutoff capability

The STM32 ECU acts as a **BMS supervisor**. It does not replace the BMS
protection hardware.

## 6.2 BMS data

The ECU should monitor:

-   Pack voltage
-   Pack current
-   State of charge
-   Minimum cell voltage
-   Maximum cell voltage
-   Minimum temperature
-   Maximum temperature
-   Discharge current limit
-   Charge current limit
-   BMS fault status
-   Contactor permission

## 6.3 BMS safety states

``` text
OK
 │
 ├── Warning / Derate
 │
 ├── Fault
 │     └── Open contactor
 │
 └── Lockout
       └── Requires service
```

------------------------------------------------------------------------

# 7. Motor and Powertrain

## 7.1 Motor

V1 target:

-   BLDC
-   48 V class
-   \~3 kW continuous
-   \~6 kW short-duration peak
-   Hall sensors
-   Winding temperature sensor
-   Air cooling
-   Suitable for FOC or sensored BLDC control

The blueprint estimates a motor speed near 4,000 rpm at the top end.

## 7.2 Motor torque

At 3 kW and 4,000 rpm:

``` text
T = P / ω

T ≈ 7.2 N·m
```

The calculated peak requirement is approximately 12 N·m at the motor, so
a design target of at least \~14 N·m peak torque is specified.

## 7.3 Gear reduction

Target:

``` text
Gear ratio ≈ 4:1
```

Example:

``` text
15T motor sprocket
        ↓
     chain
        ↓
60T axle sprocket
```

The final sprocket selection must be verified against:

-   Motor RPM
-   Wheel diameter
-   Chain rating
-   Peak torque
-   Alignment
-   Packaging
-   Tension
-   Guarding

------------------------------------------------------------------------

# 8. Mechanical Chassis

## 8.1 Chassis architecture

V1 uses a welded rigid tubular steel chassis.

Target geometry:

  Parameter               Target
  ------------------ -----------
  Wheelbase            \~1050 mm
  Front track           \~900 mm
  Rear track           \~1000 mm
  Overall length       \~1800 mm
  Overall width        \~1150 mm
  Ground clearance     40--60 mm

## 8.2 Tubing

Primary:

``` text
38 × 2.5 mm round steel tube
```

Secondary:

``` text
25 × 2 mm round steel tube
```

Suggested material basis:

-   S235 / E355 low-carbon steel
-   AISI 1020 equivalent
-   4130 chromoly is a future option but requires appropriate welding
    practice

## 8.3 Structural design

Primary tubes should carry:

-   Driver load
-   Battery load
-   Motor loads
-   Braking loads
-   Steering loads
-   Axle loads

Battery mounting should be designed around a target retention basis of
approximately **5 g**.

The blueprint's first-pass frame check gives approximately:

``` text
3 g dynamic load case
Bending stress ≈ 143 MPa
FoS ≈ 1.65 against 235 MPa yield
```

This is not a final structural validation. FEA and physical inspection
are required.

------------------------------------------------------------------------

# 9. Steering

The steering system consists of:

``` text
Steering wheel
      ↓
Steering column
      ↓
Column bearings
      ↓
Pitman arm / drag link
      ↓
Tie rods
      ↓
Steering arms
      ↓
Kingpin spindles
      ↓
Front wheels
```

Target steering characteristics:

-   Standard kart spindles
-   Ackermann geometry
-   Two steering-column bearings
-   Rod ends with locking hardware
-   Mechanical steering stops
-   No tyre-to-frame interference at full lock

The preliminary calculation gives an approximate turning radius of **2.1
m**.

------------------------------------------------------------------------

# 10. Braking System

## 10.1 V1 braking

V1 uses a rear hydraulic disc brake.

The mechanical brake must remain independent of software.

``` text
Brake pedal
    ↓
Master cylinder
    ↓
Hydraulic line
    ↓
Rear caliper
    ↓
Rear disc
```

## 10.2 Brake design basis

The first-pass analysis estimates:

-   Rear-only braking ≈ 0.41 g
-   Braking distance from 40 km/h ≈ 15.4 m
-   Additional distance is required for driver reaction time

Therefore:

> **V1 is limited to 40 km/h and front brakes are a V2 gate for higher
> speed.**

Regenerative braking must never be treated as the primary emergency
braking system.

------------------------------------------------------------------------

# 11. Pedal and Driver Inputs

## 11.1 Dual-channel throttle

The accelerator should use two independent sensor channels.

Example:

``` text
Throttle Sensor A ─┐
                   ├── STM32 ADC → Plausibility → Filter → Torque Request
Throttle Sensor B ─┘
```

The ECU checks:

-   Voltage range
-   Channel agreement
-   Rate of change
-   Startup position
-   Stuck-high condition

Any serious mismatch produces:

``` text
Torque Request = 0
```

## 11.2 Brake input

The brake input provides:

-   Brake-switch state
-   Brake light activation
-   ECU brake state
-   Optional future pressure sensing

Brake override:

``` text
Brake pressed
+
Throttle > defined threshold
        ↓
Torque cut
```

------------------------------------------------------------------------

# 12. Motor Controller / Inverter

V1 should use a **commercial motor controller** rather than building a
custom inverter.

Target class:

-   60--75 V compatible
-   \~100 A battery-side limit
-   CAN communication
-   Hall sensor input
-   Temperature monitoring
-   Current limiting
-   Over/undervoltage protection
-   Overtemperature protection
-   FOC or sensored BLDC capability

Potential commercial families mentioned in the design basis include
VESC-class, Kelly and Sabvoton-type controllers. Exact model selection
must be verified against its datasheet and CAN protocol.

### V1 rule

> **Do not build a custom high-power inverter in V1.**

A custom inverter is a later engineering project because shoot-through,
gate-drive, current sensing and EMI failures can destroy hardware
rapidly.

------------------------------------------------------------------------

# 13. Pre-Charge System

The motor controller's DC-link capacitors can appear as a near-short
circuit when the battery is connected.

Without pre-charge:

``` text
Battery → Contactor → Empty DC-link capacitor
                         ↓
                    Very high inrush
```

With pre-charge:

``` text
Battery
  ↓
Pre-charge contactor
  ↓
Resistor
  ↓
Controller DC link
```

Then the main contactor closes.

## 13.1 Preliminary values

For an assumed:

``` text
C ≈ 2,000 µF
R ≈ 100 Ω
```

The first-pass calculation gives:

``` text
τ = RC ≈ 0.2 s

≈95% after 3τ
≈0.6 s
```

The resistor energy per charge event is approximately:

``` text
E = 1/2 C V²
≈ 2.6 J at 51.2 V
```

The actual controller DC-link capacitance must be measured or obtained
from its datasheet before final resistor selection.

## 13.2 Pre-charge sequence

``` text
1. Confirm BMS permission
2. Confirm e-stop closed
3. Confirm no active faults
4. Confirm key ON
5. Confirm safe throttle position
6. Close pre-charge path
7. Monitor DC-link voltage
8. Wait for ≥95% pack voltage
9. Close main contactor
10. Verify DC-link voltage
11. Open pre-charge path
12. Enter READY
```

Timeout or abnormal voltage behaviour produces:

``` text
PRECHARGE_FAULT
```

------------------------------------------------------------------------

# 14. Low-Voltage Electrical System

A DC/DC converter provides:

``` text
51.2 V nominal HV
       ↓
48-to-12 V DC/DC
       ↓
12 V fuse distribution
```

Target converter:

``` text
~10 A
~120 W
```

12 V loads include:

-   STM32 ECU
-   Dashboard
-   Fans
-   Horn
-   Brake light
-   Lighting
-   Contactor coils
-   Other auxiliary electronics

The 12 V branches should be individually fused.

------------------------------------------------------------------------

# 15. STM32 Vehicle ECU

The STM32 ECU is the central control and safety supervisor.

## 15.1 ECU responsibilities

-   Read throttle
-   Read brake
-   Read temperatures
-   Read voltage
-   Monitor BMS
-   Monitor motor controller
-   Manage contactors
-   Manage pre-charge
-   Execute vehicle state machine
-   Calculate torque request
-   Apply safety limits
-   Handle communication timeouts
-   Operate watchdog
-   Log faults
-   Broadcast vehicle status

## 15.2 Software layers

``` text
Application
    ↓
Vehicle State Machine
    ↓
Safety Manager
    ↓
Services / Diagnostics / Logging
    ↓
CAN / BMS / Motor Controller Interfaces
    ↓
Sensors
    ↓
HAL / Drivers
    ↓
STM32 Hardware
```

## 15.3 Timing

  Function              Target rate
  ------------------- -------------
  ADC filtering               1 kHz
  Main control               100 Hz
  Controller CAN TX          100 Hz
  CAN RX processing          50 Hz+
  BMS supervision             10 Hz
  Telemetry                   10 Hz
  Housekeeping                 1 Hz

A watchdog should only be serviced when required tasks report healthy
operation.

------------------------------------------------------------------------

# 16. Vehicle State Machine

``` text
OFF
 │
 ▼
INIT
 │
 ├── fault ───────────────► FAULT
 │
 ▼
PRECHARGE
 │
 ├── failure ─────────────► PRECHARGE_FAULT
 │
 ▼
READY
 │
 ▼
DRIVE
 │
 ├── throttle released ───► REGEN (if enabled)
 │
 └── fault ───────────────► FAULT
 │
 ▼
SHUTDOWN
 │
 ▼
OFF
```

Emergency stop can interrupt the operating states:

``` text
ANY STATE
    ↓
EMERGENCY STOP
    ↓
CONTACTORS OPEN
    ↓
TORQUE = 0
    ↓
MECHANICAL BRAKE REMAINS AVAILABLE
```

------------------------------------------------------------------------

# 17. CAN Bus Architecture

CAN is the main vehicle communication backbone.

``` text
                 ┌──────────────┐
                 │     BMS      │
                 └──────┬───────┘
                        │
                        │ CAN
                        │
┌───────────┐      ┌────▼─────┐      ┌────────────────┐
│ Dashboard │◄────►│ STM32 ECU│◄────►│ Motor Controller│
└───────────┘      └────┬─────┘      └────────────────┘
                        │
                        ▼
                 Telemetry Gateway
```

Target:

-   CAN 2.0B
-   500 kbit/s
-   120 Ω termination at both ends
-   Twisted pair
-   Short stubs
-   Lower CAN ID = higher priority

Telemetry should be **listen-only** and must never be allowed to command
the vehicle.

------------------------------------------------------------------------

# 18. CAN Message Architecture

  CAN ID          Sender             Main purpose                              Rate
  --------------- ------------------ ---------------------------------- -----------
  `0x010`         ECU                System heartbeat / fault state          100 Hz
  `0x100`         BMS                Pack voltage/current/SOC                 10 Hz
  `0x101`         BMS                Cell extremes/temperatures               10 Hz
  `0x102`         BMS                Faults/current limits/permission         10 Hz
  `0x200`         ECU                Torque request/mode                     100 Hz
  `0x300`         Motor controller   RPM/currents                             50 Hz
  `0x301`         Motor controller   Temperatures/voltage/fault               10 Hz
  `0x210`         ECU                Speed/throttle/brake/state               20 Hz
  `0x211`         ECU                Faults/power/trip distance               10 Hz
  `0x7E0/0x7E8`   Tester/ECU         Diagnostics                          On demand

Command messages should use:

-   Rolling counter
-   CRC
-   Timeout detection

Example safety timeout policy:

``` text
BMS data missing > ~500 ms
        ↓
Fault / torque disabled

Motor-controller communication missing > ~200 ms
        ↓
Torque = 0
```

------------------------------------------------------------------------

# 19. Control Flow

## 19.1 Throttle

``` text
Throttle Pedal
      ↓
Dual Hall Sensors
      ↓
ADC
      ↓
Range Check
      ↓
Plausibility Check
      ↓
Filtering / Rate Limiting
      ↓
Vehicle ECU
      ↓
Torque Request
      ↓
CAN
      ↓
Motor Controller
      ↓
Electric Motor
```

## 19.2 Brake

``` text
Brake Pedal
      ↓
Brake Switch / Sensor
      ↓
Vehicle ECU
      ├──────────────► Mechanical Brake
      │
      └──────────────► Regenerative Braking Request
```

Regeneration remains secondary to the mechanical brake.

------------------------------------------------------------------------

# 20. Safety Architecture

## 20.1 Hardware emergency-stop chain

``` text
12 V supply
   ↓
E-STOP
   ↓
Key / interlock
   ↓
Contactor coil circuit
   ↓
Main contactor
```

The emergency stop must be capable of opening the contactor coil supply
independently of software.

## 20.2 Required safety hardware

-   Emergency-stop button
-   Main fuse
-   Service disconnect
-   DC-rated contactor
-   Pre-charge circuit
-   Commercial BMS
-   Brake switch
-   Mechanical brake
-   Helmet
-   Closed test area
-   Fire extinguisher
-   Insulated tools

Recommended additions:

-   Inertia/crash switch
-   12 V backup
-   Better battery enclosure
-   Future HVIL
-   Future isolation monitoring

------------------------------------------------------------------------

# 21. Fault Matrix

  Fault                        Detection               Response
  ---------------------------- ----------------------- ------------------------------------
  E-stop                       Hardware loop           Contactors open, torque 0
  Battery overvoltage          BMS                     Stop charge, disable regen/fault
  Battery undervoltage         BMS                     Derate → torque 0 → contactor open
  Battery overtemperature      BMS                     Derate → shutdown
  Motor overtemperature        Motor controller        Derate → shutdown
  Controller overtemperature   Controller              Derate → shutdown
  Throttle mismatch            ECU                     Torque 0
  Throttle stuck high          ECU                     Refuse READY
  Brake fault                  ECU                     Limit torque / fault
  Pre-charge failure           ECU                     Abort pre-charge
  Contactor weld               DC-link voltage check   LOCKOUT
  CAN timeout                  ECU/controller          Torque 0 / fault
  Watchdog reset               STM32                   Outputs default OFF
  Crash/tip-over               Inertia/IMU future      E-stop
  Unintended acceleration      RPM vs torque request   Emergency shutdown

------------------------------------------------------------------------

# 22. Charging System

V1 should use a commercial charger.

Target:

``` text
16S LiFePO4 charger
~58.4 V maximum class
~57.6 V charge target
~10 A starting charge current
```

Charging must be inhibited outside the permitted battery temperature
range.

The vehicle should not enter READY while the charge connector is
inserted.

No onboard charger is required for V1.

------------------------------------------------------------------------

# 23. Dashboard / HMI

The dashboard can use:

-   ESP32
-   TFT/OLED display
-   CAN transceiver
-   12 V-to-logic power conversion

Display:

-   Vehicle speed
-   Motor RPM
-   SOC
-   Battery voltage
-   Battery current
-   Power
-   Motor temperature
-   Battery temperature
-   Range estimate
-   Drive mode
-   Fault state
-   Charging state

The display should show:

``` text
NO DATA
```

when CAN information becomes stale.

A red fault banner should be used for safety-critical faults.

------------------------------------------------------------------------

# 24. GPS and Telemetry

Telemetry data may include:

-   GPS position
-   Speed
-   SOC
-   Voltage
-   Current
-   Power
-   Motor temperature
-   Battery temperature
-   Fault codes
-   Trip distance
-   Energy consumed

Architecture:

``` text
Vehicle ECU
    ↓
CAN
    ↓
ESP32 Gateway
    ↓
BLE / Wi-Fi
    ↓
Laptop / Phone

Optional V2:

ESP32
   ↓
MQTT over TLS
   ↓
Dashboard / Grafana / Node-RED
```

**Telemetry is never allowed to command the vehicle.**

------------------------------------------------------------------------

# 25. Data Logging

Fast channels:

-   Throttle
-   Torque request
-   RPM
-   Current

Target: \~100 Hz.

Slow channels:

-   Temperature
-   SOC
-   Cell extremes

Target: \~10 Hz.

GPS:

-   \~5 Hz target

Faults should be event-driven.

Suggested CSV fields:

``` text
timestamp_ms
speed_kmh
rpm
battery_voltage
battery_current
soc
motor_temp
throttle_pct
state
fault
```

Energy can be estimated from:

``` text
P = V × I
```

and integrated over time.

------------------------------------------------------------------------

# 26. Thermal Management

## 26.1 Motor

At approximately 3 kW output and 88% efficiency:

``` text
P_loss ≈ 409 W
```

V1 cooling:

-   Air cooling
-   Open-frame/ducted airflow
-   Temperature monitoring
-   Optional fan if testing shows excessive temperature

## 26.2 Motor controller

Estimated peak thermal loss is roughly:

``` text
~100–110 W
```

The controller should have:

-   Aluminium heat sink
-   Good thermal contact
-   Airflow
-   Optional 80--120 mm fan

## 26.3 Battery

Battery temperature must be logged during testing.

Because ambient conditions can be high, the battery enclosure should
avoid unnecessary solar heating and provide appropriate
ventilation/thermal strategy.

**Do not build liquid cooling in V1 unless test data proves air cooling
inadequate.**

------------------------------------------------------------------------

# 27. Wiring Architecture

## 27.1 Harness separation

Maintain separate harness groups:

1.  HV power
2.  12 V power
3.  Sensor wiring
4.  CAN communication
5.  Emergency-stop wiring

## 27.2 Preliminary wire sizing

  Circuit                     Preliminary wire
  ---------------------- ---------------------
  Battery → controller                  16 mm²
  Motor phase                           10 mm²
  Pre-charge                           1.5 mm²
  DC/DC input                          2.5 mm²
  12 V distribution                 2.5--4 mm²
  Contactor coils                        1 mm²
  Fans                                 \~1 mm²
  Lights/horn                        \~1.5 mm²
  ECU/dashboard                     \~0.75 mm²
  Throttle sensors         \~0.35 mm² shielded
  CAN                      AWG 22 twisted pair
  E-stop                               \~1 mm²

These are preliminary design values and must be checked against actual
current, insulation, temperature, routing and connector ratings.

## 27.3 Harness rules

-   Use proper crimped terminals
-   Use adhesive heat-shrink
-   Use abrasion protection
-   Use grommets through metal
-   Use P-clamps
-   Provide strain relief
-   Label both ends
-   Avoid sharp frame edges
-   Keep phase cables away from sensitive signals
-   Cross power/signal wiring at 90° where necessary
-   Avoid ground loops
-   Use a single star-ground philosophy
-   Terminate CAN correctly

------------------------------------------------------------------------

# 28. PCB Strategy

## V1

Use proven commercial/development modules:

-   STM32 development board
-   CAN transceiver module
-   Commercial BMS
-   Commercial motor controller
-   Commercial DC/DC
-   Commercial dashboard electronics

## V2+

Develop a custom ECU PCB containing:

-   STM32
-   CAN transceiver
-   Protected inputs
-   ADC conditioning
-   Contactor drivers
-   Sensor interfaces
-   Watchdog
-   Power protection
-   Logging interface
-   Diagnostic interface

### Rule

> Prove firmware and system behaviour before committing to a custom ECU
> PCB.

------------------------------------------------------------------------

# 29. Software Architecture

Suggested repository:

``` text
repo/
├── firmware/
│   ├── src/
│   ├── include/
│   ├── drivers/
│   └── test/
│
├── docs/
│   ├── requirements/
│   ├── architecture/
│   └── test_reports/
│
├── hardware/
│   ├── cad/
│   ├── schematics/
│   └── pcb/
│
├── tools/
│   ├── can_db.dbc
│   └── analysis.py
│
├── .github/
│   └── workflows/
│
├── CHANGELOG.md
└── README.md
```

Suggested modules:

``` text
hal
can
sensors
motor_controller
battery
bms
safety
vehicle
vehicle_state
telemetry
main
```

Software principles:

-   C++17
-   No dynamic allocation after initialization
-   No exceptions/RTTI where inappropriate for the embedded target
-   Fixed-width types
-   Hardware abstraction
-   Unit-testable logic
-   Watchdog
-   Fault-latching
-   Explicit state machine
-   Requirement-to-test traceability

------------------------------------------------------------------------

# 30. Vehicle State Machine Logic

### OFF → INIT

Conditions:

-   Key ON
-   E-stop closed

### INIT → PRECHARGE

Requirements:

-   Sensor checks pass
-   BMS heartbeat present
-   Controller heartbeat present
-   No active faults
-   Throttle near zero
-   Brake interlock valid

### PRECHARGE → READY

Requirements:

-   DC-link voltage reaches target percentage of pack voltage
-   Main contactor closes correctly
-   No contactor-weld fault

### READY → DRIVE

Driver performs anti-launch sequence:

``` text
Throttle < 5%
        +
Brake pressed
        +
Brake released
        ↓
DRIVE ENABLE
```

### ANY STATE → ESTOP

Triggered by:

-   E-stop
-   Crash input
-   Severe BMS fault
-   Severe controller fault

------------------------------------------------------------------------

# 31. Vehicle Dynamics

Total force:

``` text
F_total =
F_roll
+ F_aero
+ F_grade
+ F_acc
```

Rolling resistance:

``` text
F_roll = Crr m g cos(θ)
```

Aerodynamic drag:

``` text
F_aero = 0.5 ρ CdA v²
```

Grade force:

``` text
F_grade = m g sin(θ)
```

Acceleration force:

``` text
F_acc = k m a
```

The baseline assumptions use approximately:

``` text
m = 170 kg
Crr = 0.02
CdA = 0.5 m²
r = 0.135 m
```

These values must ultimately be measured or refined from testing.

------------------------------------------------------------------------

# 32. Performance Calculations

## 32.1 0--30 km/h

Target:

``` text
0–30 km/h ≈ 5 s
```

Required acceleration:

``` text
a ≈ 1.67 m/s²
```

The first-pass calculation produces approximately:

``` text
Peak electrical requirement ≈ 3.7 kW
Battery current ≈ 72 A
```

This supports a 100 A battery/controller design limit with margin,
subject to actual efficiency and temperature.

## 32.2 Top speed

At 50 km/h:

``` text
v = 13.89 m/s
r = 0.135 m
```

Estimated wheel speed:

``` text
≈983 rpm
```

At approximately 4,000 motor rpm:

``` text
Gear ratio ≈ 4.07
```

Therefore:

``` text
Target gear ratio ≈ 4:1
```

## 32.3 Range

Chosen usable energy basis:

``` text
1,536 Wh × 80%
≈1,229 Wh
```

At:

``` text
35 Wh/km
```

Estimated range:

``` text
≈35 km
```

Required V1 range:

``` text
≥20 km
```

Actual range must be measured from logged driving data.

------------------------------------------------------------------------

# 33. Braking Calculation

For the preliminary rear-only brake case:

``` text
Estimated deceleration ≈0.41 g
```

From 40 km/h:

``` text
Estimated braking distance ≈15.4 m
```

This calculation assumes dry-surface conditions and simplified
parameters.

The vehicle must be validated experimentally.

------------------------------------------------------------------------

# 34. Pre-Charge Calculation

For:

``` text
C = 2,000 µF
R = 100 Ω
```

Then:

``` text
τ = RC
  = 0.2 s
```

Approximately 95% charge:

``` text
3τ ≈0.6 s
```

Approximate resistor energy:

``` text
E = 1/2 CV²
≈2.6 J
```

The final resistor must be selected from the actual motor-controller
DC-link capacitance and switching/contactor requirements.

------------------------------------------------------------------------

# 35. Engineering Simulation

Use simulation where it provides high value.

  Tool                Recommended use
  ------------------- ---------------------------------------------
  Python / Excel      Dynamics, range, battery sizing, gear ratio
  MATLAB / Simulink   Control and state-machine modelling
  LTspice             Pre-charge and low-voltage circuits
  CAD / FEA           Frame, steering/brake mounts, battery mount
  Spreadsheet         Torque-speed and thermal calculations
  CFD                 Optional CdA estimation

Do not waste V1 development time simulating components that are already
commercial modules unless there is a specific engineering question to
answer.

------------------------------------------------------------------------

# 36. Manufacturing Plan

## CAD

Use:

-   Fusion 360
-   SolidWorks
-   FreeCAD

Deliver:

-   Full assembly
-   Tube cut list
-   Mounting drawings
-   Battery enclosure
-   Motor mount
-   Controller mount
-   Steering geometry
-   Brake mounts

## Fabrication

-   Cut tubes
-   Fish-mouth/notch joints
-   Fixture on flat table
-   Tack weld
-   Verify diagonals
-   Complete welds
-   Inspect critical joints

Target frame squareness:

``` text
Diagonal difference ≤ ~2 mm
```

Critical steering/brake mounts should receive additional inspection.

## Assembly order

``` text
1. Chassis
2. Rear axle
3. Steering
4. Brakes
5. Seat
6. Battery enclosure
7. Motor/controller
8. HV system
9. LV system
10. ECU
11. CAN
12. Dashboard
13. Harness
14. Testing
```

------------------------------------------------------------------------

# 37. Build Sequence

### Stage 1 --- Requirements

-   Freeze design targets
-   Verify assumptions
-   Select motor/controller/cells
-   Establish safety requirements

### Stage 2 --- Mechanical

-   CAD frame
-   FEA
-   Fabricate rolling chassis
-   Steering
-   Axle
-   Brake system
-   Seat

### Stage 3 --- HV bench

-   Battery/BMS
-   Fuse
-   Service disconnect
-   Pre-charge
-   Contactor
-   Controller
-   Motor

Perform initial testing without the vehicle moving.

### Stage 4 --- ECU

-   STM32 development board
-   ADC throttle
-   Brake input
-   CAN
-   State machine
-   Watchdog
-   Fault handling

### Stage 5 --- HIL

Simulate:

-   BMS messages
-   Motor-controller messages
-   Throttle
-   Brake
-   Faults
-   CAN timeouts

### Stage 6 --- Integration

-   Install harness
-   Install e-stop
-   Verify contactors
-   Verify CAN
-   Lifted-wheel tests

### Stage 7 --- Vehicle testing

Perform staged testing from low speed upward.

------------------------------------------------------------------------

# 38. Test Plan

## Component tests

### Motor

Acceptance:

-   Smooth rotation
-   Correct Hall sequence
-   No abnormal noise
-   Temperature remains within target

### Battery

Acceptance:

-   Cell voltage consistency
-   Capacity test
-   Protection behaviour
-   Temperature monitoring

### BMS

Test:

-   Overvoltage
-   Undervoltage
-   Overtemperature
-   Overcurrent
-   Communication loss

### Controller

Test:

-   No-load spin
-   Current limiting
-   Temperature behaviour
-   Fault reporting

### Sensors

Test:

-   Throttle sweep
-   Dual-channel agreement
-   Brake input
-   Temperature sensors

### CAN

Test:

-   Termination
-   Bitrate
-   IDs
-   Bus loading
-   Timeout behaviour
-   CRC/counter behaviour

------------------------------------------------------------------------

# 39. Vehicle Test Gates

## Gate 1 --- Static

-   Brake bleed
-   Steering check
-   Fastener check
-   E-stop
-   Contactors
-   No leaks
-   No exposed dangerous wiring

## Gate 2 --- Lifted wheels

-   Motor RPM
-   Throttle response
-   Controller communication
-   Brake switch
-   E-stop
-   Fault injection

## Gate 3 --- Low speed

Target:

``` text
≤10 km/h
```

Requirements:

-   Closed area
-   Helmet
-   Spotter
-   Smooth torque
-   Functional steering
-   Functional brake

## Gate 4 --- Medium speed

Target:

``` text
≤25 km/h
```

Requirements:

-   Repeated brake testing
-   Stable handling
-   Temperature monitoring
-   Logging

## Gate 5 --- V1 maximum

Only after all previous gates pass:

``` text
40 km/h
```

Target brake performance:

``` text
≤16 m stopping distance from 40 km/h
```

subject to test conditions and the project's acceptance procedure.

------------------------------------------------------------------------

# 40. Range Test

Procedure:

1.  Start from controlled SOC.
2.  Use a repeatable route.
3.  Log voltage/current/speed/SOC.
4.  Drive to approximately 20% SOC.
5.  Calculate Wh/km.
6.  Compare measured result with the 35 Wh/km design assumption.

The required acceptance target is:

``` text
≥20 km usable range
```

------------------------------------------------------------------------

# 41. Thermal Test

Monitor:

-   Battery temperature
-   Motor temperature
-   Controller temperature
-   Ambient temperature

V1 target limits from the design basis include:

``` text
Motor:
Derate around 80–100 °C
Cut above ~110 °C

Controller:
Derate above ~85 °C
Cut around ~100 °C

Battery:
Derate before severe thermal condition
Cut around ~60 °C
```

The actual limits must be configured from component datasheets.

------------------------------------------------------------------------

# 42. Debugging Guide

  -----------------------------------------------------------------------------------
  Symptom                 Possible cause                      First checks
  ----------------------- ----------------------------------- -----------------------
  Motor does not start    HV/precharge/throttle/Hall/e-stop   Pack V, DC-link V, CAN,
                                                              Hall wiring

  Starts then stops       Overcurrent/UV/thermal/Hall         Current, voltage sag,
                                                              temperature

  CAN failure             Wiring/termination/bitrate          60 Ω power-off,
                                                              CAN-H/L, bitrate

  BMS fault               Cell imbalance/temp/current         Cell data and BMS logs

  Overcurrent             Stall/short/incorrect limits        Mechanical drag and
                                                              controller limits

  Overtemperature         Poor cooling/high load              Temperature logs and
                                                              airflow

  Throttle fault          Wiring/sensor mismatch              Both ADC channels

  Brake fault             Switch/wiring/hydraulics            Continuity, brake
                                                              light, pressure

  Display failure         12 V/CAN/SPI                        Supply and replayed CAN

  Random reset            Brownout/EMI/watchdog               3.3 V rail, reset
                                                              reason, grounding

  EMI/noise               Poor routing/shielding              Cable routing,
                                                              filtering, grounding
  -----------------------------------------------------------------------------------

------------------------------------------------------------------------

# 43. BOM Structure

The V1 BOM should be maintained as a controlled engineering table.

## Mechanical

-   Steel chassis tubing
-   Cross-members
-   Floor
-   Seat
-   Steering wheel
-   Steering column
-   Bearings
-   Spindles
-   Tie rods
-   Rod ends
-   Rear live axle
-   Bearings/hangers
-   Wheels
-   Tyres
-   Brake disc
-   Caliper
-   Master cylinder
-   Brake lines
-   Pedals
-   Chain
-   Sprockets
-   Chain guard
-   Fasteners

## Powertrain

-   3 kW BLDC motor
-   Commercial motor controller
-   Motor Hall harness
-   Motor temperature connection
-   Motor mount
-   Gear reduction components

## Battery

-   16 LiFePO4 cells
-   Busbars
-   Cell compression hardware
-   BMS
-   Battery enclosure
-   Temperature sensors
-   Current sensor/shunt where required
-   Main fuse
-   Service disconnect
-   Main contactor
-   Pre-charge contactor/relay
-   Pre-charge resistor

## Electrical

-   48→12 V DC/DC
-   12 V fuse box
-   HV cable
-   Phase cable
-   CAN cable
-   LV wire
-   Connectors
-   Crimp terminals
-   Heat-shrink
-   Cable loom
-   Grommets
-   P-clamps
-   Labels

## Embedded

-   STM32 development board
-   CAN transceiver
-   ESP32
-   TFT display
-   GPS
-   SD card
-   IMU optional
-   Sensors
-   Watchdog / protection components

## Safety

-   E-stop button
-   Brake switch
-   Helmet
-   Fire extinguisher
-   Insulated tools
-   Warning labels

------------------------------------------------------------------------

# 44. Cost Strategy

The design basis estimates:

``` text
V1 ≈ USD 2,000–4,000
+ ~15% contingency
```

Major cost drivers:

-   Battery cells
-   Motor
-   Motor controller
-   Chassis/axle/wheels
-   Charger
-   Imported electronics
-   Local fabrication
-   Tools already owned

All prices are estimates and must be replaced with actual supplier
quotations.

------------------------------------------------------------------------

# 45. What NOT to Build in V1

The following are deliberately deferred:

-   Custom motor inverter
-   Custom BMS
-   Active cell balancing
-   Suspension
-   Liquid cooling
-   Advanced regenerative braking
-   On-board charger
-   CAN FD
-   Cloud telemetry
-   OTA firmware updates
-   Custom ECU PCB before firmware is proven
-   Dual/in-wheel motors
-   400 V architecture
-   Road-legal/homologation features

This is intentional scope control.

------------------------------------------------------------------------

# 46. V1 → V2 → V3 Roadmap

## V1 --- Working Go-Kart

Focus:

-   Mechanical integrity
-   Safe HV architecture
-   Commercial controller
-   Commercial BMS
-   STM32 ECU
-   CAN
-   Logging
-   Basic dashboard
-   40 km/h limit

## V2 --- Advanced Kart / Small EV Prototype

Upgrade:

-   Front brakes
-   Dual-circuit hydraulic braking
-   50 km/h gate
-   Custom ECU PCB
-   12 V backup
-   Inertia switch
-   Pressure-based brake sensing
-   Tuned regenerative braking
-   Improved battery enclosure
-   Telemetry/MQTT
-   Better calibration

Gate:

> V1 tests passed and ≥20 hours of logged operation without safety
> faults.

## V3 --- EV-Class Platform

Upgrade:

-   Suspension
-   Crash structure
-   96--400 V architecture study
-   Isolation monitoring
-   HVIL
-   On-board charger
-   Liquid thermal loop
-   Functional safety process
-   EMC testing
-   Advanced BMS supervision
-   Redundant control architecture

Gate:

> V2 reliability plus completed safety analysis and regulatory review.

------------------------------------------------------------------------

# 47. Project Phases

  Phase   Objective              Deliverable
  ------- ---------------------- ---------------------------------------
  0       Requirements           Signed-off specification
  1       Research               Component shortlist + preliminary BOM
  2       CAD                    Complete CAD + FEA
  3       Mechanical prototype   Rolling chassis
  4       Electrical prototype   HV system + motor spin
  5       Embedded software      STM32 ECU firmware
  6       CAN                    Network + dashboard
  7       Integration            Complete vehicle
  8       Safety                 Safety test report
  9       Testing                Vehicle test report
  10      V1 demonstration       40 km/h + range evidence
  11      Optimization           Updated calibration
  12      V2                     Front brakes + advanced electronics

------------------------------------------------------------------------

# 48. Engineering Traceability

Every major engineering claim should follow:

``` text
Requirement
    ↓
Equation
    ↓
Assumption
    ↓
Calculation
    ↓
Component Selection
    ↓
Verification Test
    ↓
Measured Result
```

Examples:

### Requirement

`REQ-001: Vehicle shall achieve 0–30 km/h in approximately 5 s.`

### Calculation

Dynamics model estimates required electrical power.

### Component

3 kW continuous / higher peak BLDC system.

### Verification

Perform five controlled 0--30 km/h tests and log:

-   Time
-   Speed
-   RPM
-   Voltage
-   Current
-   SOC
-   Temperature

------------------------------------------------------------------------

# 49. Engineering Rules

1.  Safety comes before performance.
2.  Do not drive before passing brake and e-stop tests.
3.  Do not trust an estimate when a measurement is practical.
4.  Verify component datasheets before purchasing.
5.  Verify structural calculations with FEA and inspection.
6.  Never rely on regenerative braking for emergency stopping.
7.  Never use wireless communication for safety-critical control.
8.  Use commercial BMS protection in V1.
9.  Use a commercial motor controller in V1.
10. Keep HV wiring physically protected.
11. Use proper crimped power connections.
12. Label every harness.
13. Log every test.
14. Version-control firmware.
15. Tag firmware releases before driving.
16. Tie every requirement to a test.
17. Do not add features simply because they are technically possible.
18. Keep V1 simple enough to debug.

------------------------------------------------------------------------

# 50. Safety Warning

A \~50 V battery capable of \~100 A can cause:

-   Severe burns
-   Electrical arcing
-   Fire
-   Melted tools
-   Component destruction

The kart also carries a human at speed.

Therefore:

-   Work with the service disconnect removed whenever possible.
-   Use insulated tools.
-   Protect exposed HV terminals.
-   Never work alone on an energized HV system.
-   Use a closed, supervised test area.
-   Wear appropriate PPE.
-   Do not drive without a passed brake test.
-   Use a working emergency stop.
-   Wear a helmet.
-   Keep a suitable fire extinguisher available.

This document does not replace professional electrical, mechanical,
vehicle-safety or regulatory review.

------------------------------------------------------------------------

# 51. Final Engineering Deliverables

The project should ultimately produce:

1.  Complete V1 specification
2.  CAD assembly
3.  Frame drawings
4.  Structural calculations / FEA
5.  Complete power architecture
6.  Battery/BMS documentation
7.  Wiring diagrams
8.  CAN database
9.  STM32 firmware
10. Dashboard firmware
11. Telemetry/logger software
12. BOM
13. Manufacturing drawings
14. Test procedures
15. Test results
16. Fault matrix
17. Calibration procedures
18. Maintenance procedure
19. Final engineering report
20. Demonstration data

------------------------------------------------------------------------

# 52. Final Report Structure

The final university/project report can be organized as:

1.  Cover
2.  Abstract
3.  Introduction
4.  Problem Statement
5.  Objectives
6.  Requirements
7.  System Architecture
8.  Mechanical Design
9.  Vehicle Dynamics
10. Powertrain
11. Battery
12. BMS
13. Power Electronics
14. Electrical Architecture
15. Embedded System
16. Communication Protocols
17. CAN Architecture
18. Software Architecture
19. Safety Engineering
20. Charging
21. Dashboard / HMI
22. Telemetry
23. Thermal Management
24. Wiring
25. PCB
26. BOM
27. Manufacturing
28. Testing
29. Results
30. Failure Analysis
31. Future Improvements
32. Cost Analysis
33. Conclusion
34. References
35. Appendices

Every important figure and equation should be numbered, and final
results should use measured test data rather than estimates.

------------------------------------------------------------------------

# 53. Appendices

## Appendix A --- STM32 C++ Source

Include:

-   HAL wrappers
-   CAN driver
-   Sensor drivers
-   Throttle processing
-   Brake input
-   BMS interface
-   Motor controller interface
-   Safety manager
-   Vehicle state machine
-   Contactor controller
-   Telemetry
-   Logging
-   Main scheduler

## Appendix B --- CAN Database

Include:

-   CAN IDs
-   Signal definitions
-   Scaling
-   Units
-   Rates
-   Timeouts
-   CRC/counter rules

## Appendix C --- Wiring

Include:

-   HV schematic
-   12 V schematic
-   CAN topology
-   ECU pinout
-   Connector pinouts
-   Fuse map
-   Grounding plan

## Appendix D --- BOM

Include:

-   Part number
-   Supplier
-   Quantity
-   Unit price
-   Total price
-   Datasheet
-   Status
-   Verification status

## Appendix E --- Test Results

Include:

-   Raw logs
-   Graphs
-   Test conditions
-   Firmware version
-   Battery SOC
-   Ambient temperature
-   Operator
-   Pass/fail result

## Appendix F --- Calibration

Include:

-   Throttle calibration
-   Current-sensor zero
-   Temperature checks
-   Controller calibration
-   Wheel-speed calibration

## Appendix G --- Fault Codes

Example:

  Code       Fault                      Severity
  ---------- -------------------------- ----------
  `0x0001`   Throttle sensor fault      LOW
  `0x0002`   Throttle plausibility      LOW
  `0x0004`   Brake switch fault         LOW
  `0x0008`   BMS timeout                HIGH
  `0x0010`   Motor-controller timeout   HIGH
  `0x0020`   Pre-charge fault           HIGH
  `0x0040`   Contactor weld             LOCKOUT
  `0x0080`   Battery overtemperature    HIGH
  `0x0100`   Motor overtemperature      MED
  `0x0200`   Battery UV/OV              HIGH
  `0x0400`   Emergency stop             HIGH
  `0x0800`   Crash input                HIGH
  `0x1000`   Watchdog reset             MED

## Appendix H --- CAD Drawings

-   Frame
-   Battery box
-   Motor mount
-   Steering
-   Brake mount
-   Controller mount

## Appendix I --- Assembly Instructions

Step-by-step mechanical and electrical assembly.

## Appendix J --- Maintenance

-   Pre-ride inspection
-   Fastener checks
-   Brake checks
-   Harness inspection
-   Battery inspection
-   Cell-voltage inspection
-   Periodic maintenance

------------------------------------------------------------------------

# 54. Immediate Development Priorities

## Week 1

### Days 1--2

-   Finalize requirements
-   Validate assumptions
-   Shortlist motor/controller/cells
-   Request quotations
-   Establish preliminary BOM

### Days 3--4

-   CAD chassis
-   Tube cut list
-   FEA
-   Brake/axle confirmation
-   Battery packaging

### Day 5

-   STM32 development board
-   CAN transceiver
-   Toolchain
-   GPIO test
-   ADC throttle test
-   CAN TX/RX test

### Days 6--7

Build the **e-stop + contactor + pre-charge logic on a controlled bench
setup** using low-voltage/current-limited power first.

Do **not** connect the full-power battery until the relevant bench tests
and safety checks have passed.

------------------------------------------------------------------------

# 55. Project Philosophy

The objective is not merely to make the kart move.

The objective is to create a traceable engineering system in which:

``` text
Mechanical Design
       +
Electrical Architecture
       +
Power Electronics
       +
Embedded Software
       +
CAN Communication
       +
Safety Engineering
       +
Testing
       +
Data
```

all work together as one vehicle platform.

The most valuable output is therefore not only the physical kart.

It is the engineering chain:

``` text
REQUIREMENT
    ↓
DESIGN
    ↓
CALCULATION
    ↓
IMPLEMENTATION
    ↓
TEST
    ↓
MEASUREMENT
    ↓
ANALYSIS
    ↓
REVISION
```

That is what turns the prototype from a simple electric go-kart into a
serious **mechatronics and electric-vehicle engineering project**.

------------------------------------------------------------------------

## Document Status

**Revision:** V1.0\
**Purpose:** V1 engineering design basis\
**Status:** Preliminary / verification required\
**Next controlled revision:** After CAD, component selection and initial
bench testing
