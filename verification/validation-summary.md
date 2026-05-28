
## System Description
AquaSense is a floating sensor node that measures turbidity and temperature in real time,
activates a pump-driven filter pod when turbidity exceeds a calibrated threshold,
and transmits data wirelessly via LoRa.

## Validation Tests

| Test | Method | Result | Evidence |
|---|---|---|---|
| Turbidity sensor calibration | 3-point sample calibration | Calibrated ±[X] NTU | calibration-results.md |
| Temperature sensor calibration | Ice bath + cross-reference | ±0.5°C (DS18B20 spec) | calibration-results.md |
| Pump flow rate | Timed volume collection ×3 | [X] L/h measured | flow-rate-test.md |
| Filter turbidity reduction | Pre/post treatment ×3 cycles | [X]% reduction | filter-test-results.md |
| LoRa communication | 2-board range test | Packets received at [X] m | lora_node firmware |
| LCD display | Visual check all states | All readings displayed correctly | Prototype photos |
| Pump relay trigger | Threshold test in turbid water | Pump activates within one read cycle (5s) | Serial monitor log |
| Power budget | Component datasheet calculations | ~150 mA, ~17h battery life | calculations
