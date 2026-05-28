/*
 *  — Sensor Test & Calibration Firmware
 * RAMSoc Hackathon 2026
 * Upload this, open Serial Monitor at 115200 baud, record ADC values
 * for clean water and turbid water — use them to set CAL_SLOPE in main firmware.
 */
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define TURBIDITY_PIN  34
#define ONEWIRE_PIN    4
#define SAMPLES        20

OneWire oneWire(ONEWIRE_PIN);
DallasTemperature tempSensor(&oneWire);

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("=== AquaSense Sensor Test ===");
  Serial.println("Readings update every 2 seconds.");
  Serial.println("Submerge sensor in each sample and record ADC value.");
  Serial.println("  Clean tap water   = 0 NTU reference");
  Serial.println("  Cloudy water      = ~25-50 NTU");
  Serial.println("  Very murky water  = ~100 NTU");
  Serial.println("-----------------------------");
  tempSensor.begin();
  Serial.print("DS18B20 sensors found: ");
  Serial.println(tempSensor.getDeviceCount());
  if (tempSensor.getDeviceCount() == 0)
    Serial.println("ERROR: Check DS18B20 wiring + 4.7k pullup on GPIO 4");
}

void loop() {
  long sum = 0;
  for (int i = 0; i < SAMPLES; i++) { sum += analogRead(TURBIDITY_PIN); delay(2); }
  float avg     = (float)sum / SAMPLES;
  float voltage = avg * (3.3 / 4095.0);

  tempSensor.requestTemperatures();
  float tempC = tempSensor.getTempCByIndex(0);

  Serial.println("--- Reading ---");
  Serial.print("  ADC avg:   "); Serial.println(avg, 1);
  Serial.print("  Voltage:   "); Serial.print(voltage, 4); Serial.println(" V");
  if (tempC == DEVICE_DISCONNECTED_C)
    Serial.println("  Temp:      ERROR - disconnected");
  else {
    Serial.print("  Temp:      "); Serial.print(tempC, 2); Serial.println(" C");
  }
  Serial.println();
  delay(2000);
}
