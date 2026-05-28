
## Overview
Blastoise is a modular real-time water quality monitoring platform designed to monitor, flag, and improve water conditions in WASH-related environments. Our prototype combines embedded sensing, wireless communication, and a live dashboard to provide early warning and scalable deployment.

## Problem
Unsafe water, sanitation, and hygiene systems continue to affect billions globally. Existing monitoring approaches are often manual, delayed, sparse, or too expensive for persistent deployment.

## Our Solution
We developed a minimum viable product that:
- Measures water quality in real time
- Sends readings from embedded hardware to a dashboard
- Supports future multi-node expansion using ESP32 + LoRa architecture
- Can integrate modular treatment pods and future sensor upgrades

## Key Features
- Real-time water monitoring
- Modular pod-based hardware
- ESP32-based embedded system
- LoRa-ready scalable node architecture
- Dashboard for visualization and alerts
- Future-ready sensor expansion

## System Architecture
Our system is built as a modular, node-based architecture with five main layers:

Sensor layer – Each pod contains water-quality sensors (e.g., turbidity and temperature in the MVP, with provision for additional probes such as pH, dissolved oxygen, ORP, and EC/TDS in future iterations). These sensors are mounted in direct contact with the water stream and provide continuous measurements to the controller.

Controller layer – An ESP32 microcontroller reads the sensor data, performs basic signal processing and threshold checks, and drives both the communication module and local indicators (LCD, status LEDs, and any onboard alerts). It also controls actuation elements such as the pump or valves for treatment stages.

Communication layer – Each node is designed to connect via a low-power long‑range wireless link (e.g., LoRa) to a central gateway. The controller packages sensor readings and health metrics (battery level, RSSI, uptime) into messages that are sent periodically or when thresholds are exceeded.

Power layer – The hardware is powered by a small battery and optional solar panel. A power-management stage (charging and regulation) supplies stable rails to the ESP32, sensors, communication module, and pump, while monitoring consumption to support longer-term deployment.

Treatment and actuation layer – The pod incorporates a small pump and filter/treatment cartridge. Based on sensor readings or configured rules, the controller can activate the pump to circulate water through the cartridge, enabling basic treatment now and providing a modular interface for future treatment pods (e.g., carbon filters, UV-LED units, or advanced media).

Multiple nodes can be deployed across a pond, tank, or network of water bodies. Each node sends data to a gateway, which forwards it to a dashboard for real-time visualization, alerting, and future analytics. This architecture lets the system scale from a single prototype to a distributed fleet of monitoring and treatment pods while keeping each node simple and modular.

Sensor Node -> ESP32 -> LoRa / Communication Layer -> Gateway / Dashboard  
Optional treatment path: Sensor trigger -> Pump control -> Filter / treatment pod

## Hardware
- ESP32
- [Turbidity sensor]
- [Temperature sensor]
- LCD screen
- Status LED
- Pump / motor
- Tubing and filter pod
- Battery / power system
- 3D printed enclosure

## Software
- Arduino / ESP32 firmware
- Dashboard / frontend
- Data visualization interface
- Alert / threshold logic

## Verification
Technical verification files are available in the `/verification` folder and include:
- Calculations
- Calibration data
- CAD files
- Wiring diagrams
- Test images and validation notes

## Demo
Dashboard / UX link:(https://rainbow-hydro-metric-flow.base44.app/?is_new_user=true)

## Repository Structure
- `firmware/` - embedded code
- `dashboard/` - dashboard notes and screenshots
- `electronics/` - wiring and BOM
- `cad/` - CAD files and renders
- `verification/` - calculations, validation, testing
- `slides/` - final presentation

## Team
- Ishmam Jawad Karim

## Acknowledgements
This project may incorporate open-source tools, external references, and AI-assisted workflows. All external work is acknowledged and used in accordance with academic integrity expectations.
