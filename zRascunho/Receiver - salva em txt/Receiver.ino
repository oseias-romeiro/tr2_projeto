#include <LoRa.h>
#include <SPI.h>
#include <SD.h>

// Pinos e configuração do LoRa
const int CS_PIN = 10;
const int RESET_PIN = 9;
const int IRQ_PIN = 2;

// Pinos do SD card
const int chipSelect = 4;

void setup() {
  Serial.begin(9600);

  // Inicializa LoRa
  LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Initializing OK!");

  // Inicializa SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Initialization of SD card failed!");
    while (1);
  }

  Serial.println("SD card initialized.");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("Received packet '");

      while (LoRa.available()) {
        String received = LoRa.readString();
        dataFile.print(received);
      }

      dataFile.print("' with RSSI ");
      dataFile.println(LoRa.packetRssi());
      dataFile.close(); // Fecha o arquivo após escrever
    } else {
      Serial.println("Error opening datalog.txt");
    }
  }
}
