# Smart Refrigerator – Passive Cooling Control System

## Demo Video

<div align="center">
  <iframe width="560" height="315" src="https://www.youtube.com/embed/O7e8nfwptJs" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>

## Overview
This project is an Arduino-based **Smart Refrigerator** system that automates cooling using fan-assisted heat dissipation and user-defined timing. It monitors temperature, controls fans, and provides real-time feedback through a digital display and buttons. The system is designed to improve energy efficiency during peak utility periods while maintaining safe internal temperatures.

## Project Objectives
- Automate cooling using fans and heatsinks  
- Monitor internal temperature in real time  
- Allow user input for time setting, manual override, and status control  
- Provide system feedback via digital display  
- Improve energy efficiency during peak usage periods  

## Hardware Components
- Arduino microcontroller  
- Temperature sensor (e.g., DS18B20)  
- Cooling fans and heatsinks  
- Digital display (7-segment or LCD)  
- Push buttons for time set, override, and control  
- Relays or transistor drivers  
- Power supply and wiring  

## Software and Tools
- Arduino IDE  
- C/C++ (Arduino)  
- Libraries for display and input handling  

## Data and Results
All experimental data, including temperature logs and performance results, are located in the `data/` folder (`Temperature_Data.xlsx`).

## Demonstration Videos
- Demo video stored locally: `media/Videos/Demo.mp4`  
- YouTube demo available at the top of this README.

## Project Images
Photos of the hardware setup, circuit, and final assembly are stored in `media/Images/`.

## Presentation Slides and Report
- Final technical report: `docs/Final_Report.pdf`  
- Presentation slides: `docs/Slides.pdf`  

## How It Works
1. Temperature sensor continuously monitors internal temperature  
2. Arduino reads the sensor data and compares it to predefined thresholds  
3. Fans activate automatically when cooling is required  
4. Users can manually set timing or override system using buttons  
5. System status and timing displayed in real time  
6. Safety logic prevents overheating or unsafe operation  

## Author
**Matthew Rabeje Manuel**  
Mechanical Engineering – San José State University  
Email: m.rmanuel003@gmail.com

## License
This project is for educational purposes. Free to use and modify with credit.
