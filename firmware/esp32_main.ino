/*
 *  — ESP32 Main Firmware
 * RAMSoc Hackathon 2026
 *
 * Reads turbidity + temperature, drives LCD, pump relay,
 * RGB status LED, and transmits via LoRa.
 *
 * Libraries needed (Arduino Library Manager):
 *   OneWire by Paul Stoffregen
 *   DallasTemperature by Miles Burton
 *   LiquidCrystal_I2C by John Rickman
 *   LoRa by Sandeep Mistry
 */

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <LoRa.h>

// ── Pin Definitions ──────────────────────────────────────────────────────────
#define TURBIDITY_PIN     34
#define ONEWIRE_PIN       4
#define PUMP_RELAY_PIN    26
#define LED_RED_PIN       25
#define LED_GREEN_PIN     27
#define LED_BLUE_PIN      32

#define LORA_SS_PIN       5
#define LORA_RST_PIN      14
#define LORA_DIO0_PIN     2

// ── Thresholds ───────────────────────────────────────────────────────────────
// Clean tap water ≈ 3800 ADC; turbid water < 1000 ADC
// 20 NTU threshold ≈ ADC 3820 - (20/0.03448) = ~3240 ADC
#define TURBIDITY_THRESHOLD_NTU  20.0
#define TURBIDITY_SAMPLES        10
#define TEMP_HIGH_ALERT          30.0
#define TEMP_LOW_ALERT           5.0

// ── Timing ───────────────────────────────────────────────────────────────────
#define READ_INTERVAL_MS    5000
#define PUMP_ON_DURATION_MS 30000
#define LORA_TX_INTERVAL_MS 10000

// ── Calibration constants (update after running sensor_test calibration) ─────
const float CAL_ADC_BASELINE = 3820.0;   // ADC at 0 NTU clean water
const float CAL_SLOPE        = 0.03448;  // NTU per ADC unit

// ── Objects ──────────────────────────────────────────────────────────────────
OneWire oneWire(ONEWIRE_PIN);
DallasTemperature tempSensor(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change to 0x3F if LCD blank

// ── State ────────────────────────────────────────────────────────────────────
float currentNTU   = 0.0;
float currentTempC = 0.0;
bool  pumpRunning  = false;
unsigned long pumpStartTime  = 0;
unsigned long lastReadTime   = 0;
unsigned long lastLoRaTxTime = 0;
int   packetCount = 0;

// ── Prototypes ────────────────────────────────────────────────────────────────
float readTurbidityNTU();
float readTemperatureC();
void  updateLCD(float ntu, float temp, bool pump);
void  updateLED(float ntu, float temp);
void  controlPump(float ntu);
void  transmitLoRa(float ntu, float temp);

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("AquaSense Node Starting...");

  pinMode(PUMP_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, HIGH);  // Active-low: HIGH = pump OFF
  pinMode(LED_RED_PIN,   OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN,  OUTPUT);
  digitalWrite(LED_GREEN_PIN, HIGH);   // Green on boot

  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("AquaSense v1.0");
  lcd.setCursor(0, 1); lcd.print("Initialising...");
  delay(2000);

  tempSensor.begin();
  Serial.print("DS18B20 sensors: ");
  Serial.println(tempSensor.getDeviceCount());

  LoRa.setPins(LORA_SS_PIN, LORA_RST_PIN, LORA_DIO0_PIN);
  if (!LoRa.begin(433E6)) {
    Serial.println("ERROR: LoRa init failed. Check wiring.");
    lcd.clear(); lcd.print("LoRa FAIL");
    while (true) {
      digitalWrite(LED_RED_PIN, HIGH); delay(300);
      digitalWrite(LED_RED_PIN, LOW);  delay(300);
    }
  }
  LoRa.setSpreadingFactor(10);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  Serial.println("LoRa OK");

  lcd.clear(); lcd.print("System Ready");
  delay(1500); lcd.clear();
  Serial.println("Setup complete.");
}

// ─────────────────────────────────────────────────────────────────────────────
void loop() {
  unsigned long now = millis();

  if (now - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = now;
    currentNTU   = readTurbidityNTU();
    currentTempC = readTemperatureC();
    Serial.print("NTU: "); Serial.print(currentNTU, 1);
    Serial.print("  Temp: "); Serial.print(currentTempC, 2); Serial.println(" C");
    updateLCD(currentNTU, currentTempC, pumpRunning);
    updateLED(currentNTU, currentTempC);
    controlPump(currentNTU);
  }

  if (pumpRunning && (now - pumpStartTime >= PUMP_ON_DURATION_MS)) {
    digitalWrite(PUMP_RELAY_PIN, HIGH);
    pumpRunning = false;
    Serial.println("Pump cycle complete. Pump OFF.");
  }

  if (now - lastLoRaTxTime >= LORA_TX_INTERVAL_MS) {
    lastLoRaTxTime = now;
    transmitLoRa(currentNTU, currentTempC);
  }
}

// ── Turbidity: 20-sample ADC average → NTU ───────────────────────────────────
float readTurbidityNTU() {
  long sum = 0;
  for (int i = 0; i < TURBIDITY_SAMPLES; i++) {
    sum += analogRead(TURBIDITY_PIN);
    delay(5);
  }
  float avg = (float)sum / TURBIDITY_SAMPLES;
  avg = constrain(avg, 0, 4095);
  float ntu = CAL_SLOPE * (CAL_ADC_BASELINE - avg);
  return max(ntu, 0.0f);
}

// ── DS18B20 temperature ────────────────────────────────────────────────────────
float readTemperatureC() {
  tempSensor.requestTemperatures();
  float t = tempSensor.getTempCByIndex(0);
  if (t == DEVICE_DISCONNECTED_C) {
    Serial.println("WARNING: DS18B20 disconnected.");
    return -99.0;
  }
  return t;
}

// ── LCD display ───────────────────────────────────────────────────────────────
void updateLCD(float ntu, float temp, bool pump) {
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("T:"); lcd.print(ntu, 1); lcd.print("NTU");
  if (pump) { lcd.setCursor(9, 0); lcd.print("PUMP ON"); }
  lcd.setCursor(0, 1); lcd.print("Temp:"); lcd.print(temp, 1);
  lcd.write((uint8_t)223); lcd.print("C");
}

// ── RGB LED: green=OK, yellow=caution, red=alert ──────────────────────────────
void updateLED(float ntu, float temp) {
  bool alert  = (ntu > TURBIDITY_THRESHOLD_NTU * 2) ||
                (temp > TEMP_HIGH_ALERT) ||
                (temp < TEMP_LOW_ALERT && temp > -90);
  bool caution = (ntu > TURBIDITY_THRESHOLD_NTU);

  if (alert) {
    digitalWrite(LED_RED_PIN, HIGH); digitalWrite(LED_GREEN_PIN, LOW);  digitalWrite(LED_BLUE_PIN, LOW);
  } else if (caution) {
    digitalWrite(LED_RED_PIN, HIGH); digitalWrite(LED_GREEN_PIN, HIGH); digitalWrite(LED_BLUE_PIN, LOW);
  } else {
    digitalWrite(LED_RED_PIN, LOW);  digitalWrite(LED_GREEN_PIN, HIGH); digitalWrite(LED_BLUE_PIN, LOW);
  }
}

// ── Pump control ──────────────────────────────────────────────────────────────
void controlPump(float ntu) {
  if (!pumpRunning && ntu > TURBIDITY_THRESHOLD_NTU) {
    Serial.println("Threshold breached. Pump ON.");
    digitalWrite(PUMP_RELAY_PIN, LOW);  // Active-low: LOW = pump ON
    pumpRunning  = true;
    pumpStartTime = millis();
  }
}

// ── LoRa transmit: "NODE_001,<ntu>,<temp>,<pump>,<id>" ──────────────────────
void transmitLoRa(float ntu, float temp) {
  String pkt = "NODE_001," + String(ntu, 1) + "," +
               String(temp, 2) + "," +
               (pumpRunning ? "1" : "0") + "," +
               String(packetCount++);
  LoRa.beginPacket();
  LoRa.print(pkt);
  LoRa.endPacket();
  Serial.print("LoRa TX: "); Serial.println(pkt);
}
