/*
 *  — LoRa Link Test
 * RAMSoc Hackathon 2026
 * Set TX_MODE true on one board, false on the other.
 * LoRa wiring: MOSI=23, MISO=19, SCK=18, NSS=5, RST=14, DIO0=2
 */
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define TX_MODE       true    // Change to false on the receiver board
#define LORA_SS       5
#define LORA_RST      14
#define LORA_DIO0     2
#define FREQUENCY     433E6

int count = 0;

void setup() {
  Serial.begin(115200);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(FREQUENCY)) {
    Serial.println("LoRa FAILED. Check wiring. VCC must be 3.3V only.");
    while (true);
  }
  LoRa.setSpreadingFactor(10);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
  Serial.println(TX_MODE ? "TRANSMITTER ready" : "RECEIVER ready");
}

void loop() {
  if (TX_MODE) {
    String msg = "AQUASENSE,PKT:" + String(count++) + ",NTU:14.3,TEMP:22.5";
    Serial.print("TX: "); Serial.println(msg);
    LoRa.beginPacket(); LoRa.print(msg); LoRa.endPacket();
    delay(5000);
  } else {
    int size = LoRa.parsePacket();
    if (size) {
      String rx = "";
      while (LoRa.available()) rx += (char)LoRa.read();
      Serial.print("RX: "); Serial.println(rx);
      Serial.print("  RSSI: "); Serial.print(LoRa.packetRssi()); Serial.println(" dBm");
    }
  }
}
