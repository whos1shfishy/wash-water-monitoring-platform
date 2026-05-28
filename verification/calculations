# Engineering Calculations — BLASTOISE

## 1. Pump Flow Rate & Treatment Capacity

**Pump spec:** 5V submersible, rated 80–120 L/h
**Conservative value used:** Q = 80 L/h = 22.2 mL/s

**Filter pod void volume:**
Filter pod internal volume ≈ 150 mL
GAC void fraction ≈ 0.40
Void volume = 150 × 0.40 = 60 mL

**Empty Bed Contact Time (EBCT):**
EBCT = 60 mL ÷ (80,000 mL/h) = 0.045 min = 2.7 seconds

**Volume treated per pump cycle (30 seconds):**
V = 22.2 mL/s × 30 s = 666 mL ≈ 0.67 L per activation

Note: Industrial activated carbon systems use 5–20 min EBCT.
Our pod is a proof-of-concept treatment demonstration.

---

## 2. Power Budget

| Component | Current (mA) |
|---|---|
| ESP32 active | 80 |
| Turbidity sensor | 30 |
| DS18B20 | 1.5 |
| LCD backlight | 20 |
| LoRa standby | 1.5 |
| RGB LED (1 channel) | 15 |
| **Total** | **~148 mA** |

**Power = 3.3V × 0.150 A = 0.495 W ≈ 0.5 W**

**Battery life (18650, 3000 mAh):**
Runtime = 3000 mAh ÷ 150 mA = 20 hours
With 15% efficiency deduction: ~17 hours

**Solar panel (5V, 1W, 5 peak sun hours/day in eastern Australia):**
Daily solar energy = 1W × 5h = 5 Wh
Daily node consumption = 0.5W × 24h = 12 Wh
Solar offset = 5/12 = 41.7% — significantly extends battery life

---

## 3. Turbidity Calibration Model

Calibration samples (recorded during calibration procedure):
- Sample A: Tap water (0 NTU) → ADC avg ≈ 3820, Voltage ≈ 3.08V
- Sample C: Turbid water (~100 NTU) → ADC avg ≈ 820, Voltage ≈ 0.66V

**Linear fit:**
Slope = (100 − 0) ÷ (3820 − 820) = 100 ÷ 3000 = 0.0333 NTU/ADC unit

**Firmware formula:**
NTU = 0.0333 × (3820 − ADC_reading)

**Pump activation threshold:** 20 NTU
Corresponding ADC ≈ 3820 − (20 ÷ 0.0333) ≈ 3220

---

## 4. LoRa Range Estimate

Settings: SF10, BW 125 kHz, TX power 17 dBm (Ra-02 module)
Receiver sensitivity at SF10: approximately −133 dBm
Antenna gain: ~2 dBi each end

Link budget = 17 + 2 + 2 + 133 = **154 dB**

Using Friis equation at 433 MHz (λ = 0.693 m):
d = (0.693 / 4π) × 10^(154/20)
d ≈ 0.0552 × 10^7.7 ≈ **2,766 m (line of sight ideal)**

Practical open-water range: 1–3 km
Tested range (record yours): [X] metres

---

## 5. Carbon Filter Replacement Estimate

GAC mass in pod: ~80–100 g
GAC adsorption capacity (general organics): ~200 mg/g
Total capacity = 80g × 200 mg/g = 16,000 mg = 16 g

At TOC ≈ 5 mg/L (typical surface water):
Volume treated before exhaustion = 16,000 mg ÷ 5 mg/L = 3,200 L

At 80 L/h pump throughput:
Filter life = 3,200 ÷ 80 = **~40 hours of active pump runtime**
