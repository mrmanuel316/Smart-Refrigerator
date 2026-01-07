# Smart Refrigerator – Passive Cooling Control System

## Overview
This project is an Arduino-based smart refrigerator control system designed to automate cooling using fan-assisted heat dissipation and user-defined timing. The system monitors temperature, controls fans and actuators, and provides real-time feedback through a digital display and button interface. The goal is to improve cooling efficiency during peak utility periods while maintaining safe internal temperatures.

This project demonstrates skills in embedded systems, thermal management, sensor integration, control logic, and user interface design.

## Project Objectives
- Automate cooling using fan control and heatsinks
- Monitor internal temperature in real time
- Allow user input for time setting, system override, and status control
- Provide clear feedback via digital display
- Improve energy efficiency during peak utility periods

## Hardware Components
- Arduino microcontroller
- Temperature sensor (e.g. DS18B20 or equivalent)
- Cooling fans
- Heatsinks
- Digital display (TM1637 / 7-segment or LCD)
- Push buttons (time set, override, control)
- Relays / transistors for fan control
- Power supply
- Wiring and connectors

*Exact part numbers and wiring diagrams are provided in the documentation folder.*

## Software and Tools
- Arduino IDE
- C/C++ (Arduino)
- Embedded control logic
- Display and input handling libraries

## System Features
- Real-time temperature monitoring
- Automatic fan activation based on control logic
- User interface with buttons for time setting and manual override
- Digital display output for system status and timing
- Safety control logic to prevent overheating

## System Architecture
*Insert system block diagram image here.*  
Example path: `docs/images/system_architecture.png`

## Repository Structure
Smart-Refrigerator/
├── code/
│ ├── main.ino
│ └── libraries/
├── docs/
│ ├── slides/
│ ├── report/
│ └── wiring_diagrams/
├── media/
│ ├── images/
│ └── videos/
├── data/
│ └── temperature_logs/
└── README.md

## Data and Results
All experimental data, temperature logs, and performance results are stored in the `data/` directory.  

*Insert plots, tables, or screenshots here.*  
Example path: `data/temperature_logs/`

## Demonstration Videos
Project demo videos showing system operation, user interface, and fan control are located in:

`media/videos/`

*You may also embed external video links here.*

## Project Images
Photos of the hardware setup, wiring, and final build are located in:

`media/images/`

## Presentation Slides and Report
- Final presentation slides: `docs/slides/`  
- Technical report: `docs/report/`

## How It Works
1. The temperature sensor continuously reads internal refrigerator temperature
2. The microcontroller processes the data and compares it to preset thresholds
3. If cooling is required, the system activates the fans
4. The user can set timing and override modes using push buttons
5. System status and timing are displayed on the digital interface
6. Safety logic ensures reliable operation

## What I Learned
- Embedded system integration (sensors, actuators, display)
- Real-time control logic design
- Hardware-software debugging
- User interface implementation in constrained systems
- Thermal management concepts

## Future Improvements
- Add closed-loop PID temperature control
- Integrate data logging to SD card or cloud storage
- Add mobile app or Bluetooth control
- Improve enclosure design and cable management
- Optimize power consumption

## Author
Matthew Rabeje Manuel  
Mechanical Engineering – San José State University  
Email: m.rmanuel003@gmail.com

## License
This project is for educational purposes. Feel free to use and modify with credit.
