#include "SPI.h"
#include "LoRa.h"
#include "Ultrasonic.h"

// Pinos e configuração do Ultrassom
#define TRIGGER_PIN 3;
#define ECHO_PIN 4;
#define PULSE_TIMEOUT 5000UL;
#define TIME_TO_SLEEP 10000UL;

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN, PULSE_TIMEOUT);
int distance;

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
  distance = ultrasonic.read();

  Serial.print("Distance in cm: ");
  Serial.println(distance);

  LoRa.beginPacket();
  LoRa.print(distance);
  LoRa.endPacket();

  delay(TIME_TO_SLEEP);
}
