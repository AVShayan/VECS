# VECS — Vehicle Embedded Control System

![Project Status](https://img.shields.io/badge/status-active_development-blue)
![Domain](https://img.shields.io/badge/domain-Automotive%20Embedded%20Systems-red)
![Platform](https://img.shields.io/badge/platform-ESP32%20%7C%20STM32-green)

## Overview

**VECS (Vehicle Embedded Control System)** is an automotive-inspired embedded systems platform developed to explore and implement modern **Electric Vehicle (EV) electronic architectures**.

The goal of this project is to design a modular vehicle ecosystem consisting of multiple Electronic Control Units (ECUs), communication networks, diagnostics, and connected vehicle features while following principles used in modern automotive systems.

The project focuses on bridging the gap between embedded systems development and automotive software engineering by implementing concepts such as:

- ECU-based architecture
- CAN communication
- Vehicle state management
- Diagnostics and fault handling
- Telematics integration
- Embedded software architecture
- Connected vehicle systems


---

# Vision

Modern vehicles are evolving into **software-defined platforms** where multiple ECUs communicate and coordinate vehicle functions.

VECS aims to build a miniature automotive electronic architecture consisting of:

```
                    Mobile Application
                           |
                           |
                    Telematics ECU
                           |
                         CAN Bus
                           |
        -----------------------------------------
        |                  |                    |
       BCM                VCU            Dashboard ECU
        |                  |
        |                  |
  Vehicle Body       Vehicle Control
  Functions          Functions
        |
        |
 Lights / Horn / Indicators
```

The project follows a scalable architecture where additional ECUs and features can be integrated without redesigning the complete system.


---

# Current Implementation

## Body Control Module (BCM)

The first subsystem developed under VEDS is the **Body Control Module**.

The BCM manages vehicle body electronics and provides centralized control over:

### Implemented Features

✅ Headlight control  
✅ Indicator control  
✅ Horn control  
✅ Brake light control  


### Current Hardware

- ESP32 / STM32 Microcontrollers
- Automotive accessory circuits
- Relay-based switching
- 12V vehicle electrical system
- Custom wiring and integration


---

# Software Architecture

VECS follows a layered embedded software architecture inspired by automotive ECU development practices.

```
+--------------------------------+
|       Application Layer        |
|                                |
| Lighting Manager               |
| Indicator Manager              |
| Vehicle Logic                  |
+--------------------------------+

+--------------------------------+
|        Service Layer           |
|                                |
| State Manager                  |
| Diagnostics                    |
| Communication                  |
+--------------------------------+

+--------------------------------+
|       Hardware Layer           |
|                                |
| GPIO Drivers                   |
| ADC Drivers                    |
| PWM Drivers                    |
+--------------------------------+
```

The objective is to maintain modularity, scalability, and hardware abstraction similar to professional embedded systems.


---

# Development Roadmap

## Phase 1 — Body Control Module ✅

Completed:

- Accessory control
- Relay-based switching
- Basic vehicle functions


## Phase 2 — Automotive BCM Architecture 🚧

Planned:

- Vehicle state machine
- Non-blocking scheduler
- Fault management
- Diagnostic framework
- Sleep/wakeup management


## Phase 3 — Vehicle Communication

Planned:

- CAN bus integration
- ECU-to-ECU communication
- CAN message database
- Signal-based communication


## Phase 4 — Vehicle Control Unit (VCU)

Planned:

- Drive modes
- Throttle processing
- Vehicle control logic
- Motor controller integration


## Phase 5 — Telematics System

Planned:

- Mobile application connectivity
- Remote vehicle commands
- "Ping My Vehicle" feature
- GPS tracking
- Vehicle telemetry


## Phase 6 — Advanced Automotive Features

Future exploration:

- OTA firmware updates
- Diagnostics (UDS concepts)
- ECU gateway
- Automotive Ethernet concepts
- AUTOSAR-inspired architecture


---

# Automotive Concepts Explored

VECS is designed as a practical learning platform for:

| Concept | Status |
|---|---|
| ECU Architecture | 🚧 In Development |
| BCM Design | ✅ Started |
| CAN Communication | Planned |
| Diagnostics | Planned |
| Telematics | Planned |
| Vehicle State Machine | Planned |
| AUTOSAR Concepts | Future |
| Functional Safety Concepts | Future |


---

# Hardware Platform

Current:

- ESP32
- STM32 (future ECU development)
- Relay modules
- DC-DC power conversion
- Electric vehicle platform


Future:

- CAN transceivers
- Automotive-grade communication modules
- Dedicated ECU PCBs


---

# Project Goals

VECS aims to achieve:

- A modular EV electronic architecture
- Real-time embedded vehicle control
- Distributed ECU communication
- Automotive-style software organization
- Hands-on understanding of modern vehicle systems


---

# Learning Objectives

Through VECS, the following domains are explored:

### Embedded Systems
- Embedded C/C++
- Microcontroller programming
- Hardware abstraction
- Real-time software design

### Automotive Systems
- ECU architecture
- CAN networks
- Diagnostics
- Vehicle communication

### Connected Vehicles
- Telematics
- Mobile integration
- Remote vehicle interaction


---

# Repository Structure

```
VECS/

├── docs/
│   ├── architecture/
│   ├── design/
│   └── roadmap/

├── firmware/
│   ├── bcm/
│   ├── vcu/
│   └── telematics/

├── hardware/
│   ├── schematics/
│   ├── pcb/
│   └── wiring/

├── communication/
│   └── can_database/

└── testing/
```

---

# Author

Developed as a personal exploration into:

**Automotive Embedded Systems | EV Architecture | ECU Development | Connected Vehicles**

---

# License

This project is currently for educational and research purposes.
