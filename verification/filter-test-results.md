

**Test date:** [28/5/26]  
**Operator:** [Ishmam]

## Purpose
Verify that the activated carbon filter pod reduces turbidity in test water.

## Setup
- 2L test water: tap water + kaolin clay or potting mix stirred in
- AquaSense node running esp32_main.ino
- Serial monitor logging NTU readings

## Procedure
1. Added turbid matter to test water until sensor read >20 NTU (pump threshold)
2. Recorded pre-treatment NTU from serial monitor
3. Allowed 1 × 30-second pump cycle to complete
4. Waited 30 seconds for re-mixing, recorded post-treatment NTU
5. Repeated for 3 cycles

## Results

| Cycle | Pre-Treatment (NTU) | Post-Treatment (NTU) | Reduction (NTU) | Reduction (%) |
|---|---|---|---|---|
| 1 | [RECORD] | [RECORD] | [diff] | [%] |
| 2 | [RECORD] | [RECORD] | [diff] | [%] |
| 3 | [RECORD] | [RECORD] | [diff] | [%] |

- test-photos/filter-before.jpg — visibly cloudy test water
- test-photos/filter-after.jpg  — water after pump cycles (noticeably clearer)
- test-photos/filter-monitor.jpg — serial monitor showing NTU values before and after

## Interpretation
The filter pod achieved approximately [X]% turbidity reduction per cycle.
Caveat: GAC is primarily effective for dissolved organics; mechanical filtration
through the GAC bed contributes to turbidity reduction for suspended particles.
