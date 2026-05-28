# Electronics — Wiring Guide

## Wiring Table

| Component | ESP32 Pin | Notes |
|---|---|---|
| Turbidity sensor SIG | GPIO 34 | ADC1 only — do not use ADC2 pins |
| Turbidity sensor VCC | 3.3V or 5V | Check your module |
| Turbidity sensor GND | GND | |
| DS18B20 DATA | GPIO 4 | 4.7kΩ pull-up to 3.3V REQUIRED |
| DS18B20 VCC | 3.3V | |
| DS18B20 GND | GND | |
| LCD SDA | GPIO 21 | |
| LCD SCL | GPIO 22 | |
| LCD VCC | 5V | |
| LoRa MOSI | GPIO 23 | |
| LoRa MISO | GPIO 19 | |
| LoRa SCK | GPIO 18 | |
| LoRa NSS | GPIO 5 | |
| LoRa RESET | GPIO 14 | |
| LoRa DIO0 | GPIO 2 | |
| LoRa VCC | 3.3V | NEVER 5V — will destroy module |
| Relay IN | GPIO 26 | Active LOW |
| Relay VCC | 5V | |
| Relay COM | Pump+ | |
| Relay NO | Power+ | |
| Pump− | GND | |
| LED Red | GPIO 25 | 220Ω resistor in series |
| LED Green | GPIO 27 | 220Ω resistor in series |
| LED Blue | GPIO 32 | 220Ω resistor in series |

## DS18B20 Pull-Up (Critical)
3.3V ──── 4.7kΩ ──── GPIO 4 (and DS18B20 DATA pin)
Without this resistor the sensor will not work.

## LCD Address
Default: 0x27. If nothing shows, try 0x3F in the code.

## Safety
- Never run pump dry
- LoRa module MUST be 3.3V — 5V will destroy it
- ESP32 GPIO max current: 40mA per pin
