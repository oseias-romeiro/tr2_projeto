#include <LoRa.h>

// Pinos e configuração do LoRa
const int CS_PIN = 10;
const int RESET_PIN = 9;
const int IRQ_PIN = 2;

void setup() {
  Serial.begin(9600);

  // Inicializa LoRa
  LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Initializing OK!");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    Serial.print("Received packet '");

    while (LoRa.available()) {
      String received = LoRa.readString();
      Serial.print(received);
    }

    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

}
