# Sensor Calibration Results — BLASTOISE
RAMSoc Hackathon 2026

**Calibration date:** [27/5/2026]
**Operator:** [Ishmam Jawad Karim]
**Firmware used:** sensor_test.ino

---

## Turbidity Sensor Calibration

### Equipment
- ESP32 + turbidity sensor wired per circuit-notes.md
- sensor_test.ino firmware, Serial Monitor 115200 baud
- Three water samples (described below)

### Samples and Results

| Sample | Description | ADC Avg (20 samples) | Voltage (V) | NTU (assigned) |
|---|---|---|---|---|
| A | Tap water, clear | [FILL IN] | [FILL IN] | 0 NTU |
| B | Diluted flour/clay, slight haze | [FILL IN] | [FILL IN] | ~25 NTU |
| C | Murky, heavily turbid | [FILL IN] | [FILL IN] | ~100 NTU |



### Calibration Curve (from your values)
Slope = (100 − 0) ÷ (ADC_A − ADC_C) = [CALCULATED]
NTU = Slope × (ADC_A − ADC_reading)

Cross-check at Sample B:
  Predicted NTU = [calculated]
  Expected: ~25 NTU
  Error: [calculated]%

### Sensor Stability
- Settled within ~15 seconds of submersion
- Reading noise: ±[X] NTU at steady state

---

## Temperature Sensor Calibration (DS18B20)

### Ice Bath Test (0°C reference)
Procedure: crushed ice + tap water, equilibrated 5 min, DS18B20 fully submerged.

| Measurement | Value |
|---|---|
| DS18B20 reading | [RECORD] °C |
| Expected (ice point) | 0.0 °C |
| Error | [difference] °C |

📷 PHOTO REQUIRED: Serial monitor showing DS18B20 in ice bath.
Save as: test-photos/temperature-cal.jpg

### Tap Water Cross-Check
| Reading | Value |
|---|---|
| DS18B20 | [RECORD] °C |
| Reference thermometer (if available) | [RECORD] °C |
| Offset | [difference] °C |

DS18B20 factory spec: ±0.5°C. No firmware correction applied.
Measured accuracy: ±[X] °C.

---

## Threshold Setting
Based on calibration: pump activates at **20 NTU**
Corresponding ADC value: [ADC_A − (20 ÷ slope)] = [CALCULATED]
Updated in esp32_main.ino as TURBIDITY_THRESHOLD_NTU = 20.0

**Calibration sign-off:** [Name], [Date]
