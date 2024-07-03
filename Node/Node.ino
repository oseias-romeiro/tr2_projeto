#include <Ultrasonic.h>
#include <SPI.h>
#include <LoRa.h>

// COM6 (DIREITA)

#define PIN_TRIG 3
#define PIN_ECHO 4
#define PULSE_TIMEOUT 5000UL

Ultrasonic ultrasonic(PIN_TRIG, PIN_ECHO, PULSE_TIMEOUT);
const String TANQUE_ID = "1001";

// Retorna true quando receber sinal do gateway
bool waitingGatewaySignal() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;  // Nenhum pacote disponível
  return false;
}

// Envia o ID como sinal de que está acordado
void sendSignal() {
  LoRa.beginPacket();
  LoRa.print(TANQUE_ID);
  LoRa.endPacket();
}

// Retorna true quando o gateway confirmar o ID
bool establishConnection() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return false; // Nenhum pacote disponível
  String resposta = "";
  while (LoRa.available()) {
    resposta += (char)LoRa.read();
  }
  if (resposta.equals(TANQUE_ID)) {
    Serial.println("Gateway confirmou o ID: " + resposta);
    return true;
  }else {
    Serial.println("não é minha vez");
    delay(10000); // Aguarda 10 segundos antes de tentar novamente
    return false;
  }
}

// Envia a distância medida para o gateway
void sendDistance(long distance) {
  LoRa.beginPacket();
  LoRa.print(String(distance));
  LoRa.endPacket();
  Serial.println("Enviado dado: " + String(distance));
}

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
}

void loop() {
  // Aguarda sinal do gateway
  while (waitingGatewaySignal()) {
    Serial.println("Aguardando sinal do gateway...");
  }
  delay(2000);
  // Aguarda confirmação de conexão pelo gateway
  while(true) {
    sendSignal();
    Serial.println("Enviado ID para o gateway: " + TANQUE_ID);
    // Espera a resposta do gateway
    int timeout = 10000; // Tempo máximo de espera (10 segundos)
    unsigned long start = millis();
    bool ok = false;
    while (millis() - start < timeout) {
      if(establishConnection()){
        ok = true;
        break;
      }
    }
    if(ok) break;
    else Serial.println("Falha, tentando novamente...");
  }
  delay(2000);
  // Realiza a medição
  long distance = ultrasonic.read();

  // Envia a distância medida para o gateway
  sendDistance(distance);

  // Dorme por 30 segundos antes de repetir o ciclo
  Serial.println("Dormindo...");
  delay(30000);
}
