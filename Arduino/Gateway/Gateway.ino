#include <SPI.h>
#include <LoRa.h>

String id_connected = "";
String data_received = "";
bool debug = false;

// Retorna false quando receber o ID do nó
bool waitingNodeID() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;  // Nenhum pacote disponível
  while (LoRa.available()) {
    id_connected += (char)LoRa.read();
  }
  return false;
}

// Envia sinal de broadcast
void sendBroadcastSignal() {
  LoRa.beginPacket();
  LoRa.print(" ");
  LoRa.endPacket();
}

// Envia sinal para o nó conectado
void establishConnection() {
  LoRa.beginPacket();
  LoRa.print(id_connected);
  LoRa.endPacket();
}

// retorna false quando receber os dados do nó conectado
bool waitingNodeData() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;  // Nenhum pacote disponível
  while (LoRa.available()) {
    data_received += (char)LoRa.read();
  }
  return false;
}

// Envia dados recebidos via serial
void sendData() {
  Serial.println("{\"id\": \"" + id_connected + "\", \"distance\": \"" + data_received + "\"}");
}

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
  pinMode(12, INPUT);
}

void loop() {
  // Envia broadcast e aguarda o ID do nó
  while(true){
    if(debug) Serial.println("Enviado sinal de broadcast");
    sendBroadcastSignal();
    delay(1000);
    // Espera a resposta de um nó
    int timeout = 10000; // Tempo máximo de espera (10 segundos)
    unsigned long start = millis();
    bool ok = false;
    while(millis() - start < timeout){
      if(!waitingNodeID()){
        ok = true;
        if(debug) Serial.println("Id recebido: " + id_connected);
        break;
      };
    }
    if(ok) break;
  }
  // Aguarda sinal do nó e recebe os dados
  delay(2000);
  int c = 0;
  while (true) {
    if(debug) Serial.println("Conectando com o nó " + id_connected);
    establishConnection();
    delay(1000);
    // Aguarda os dados do nó
    int timeout = 10000; // Tempo máximo de espera (10 segundos)
    unsigned long start = millis();
    bool ok;
    while(millis() - start < timeout){
      if(!waitingNodeData()) {
        ok = true;
        if(debug) Serial.println("Dados recebidos do nó: " + data_received);
        break;
      }
    }
    if(ok) break;
    if(debug) Serial.println("Falha, tentando novamente...");
    c++;
    if(c > 3) break;
  }

  // Envia dados via serial
  sendData();

  // Limpa variáveis para o próximo ciclo
  id_connected = "";
  data_received = "";

  if(debug) Serial.println("Dormindo...");
  delay(30000);  // Espera 30 segundos antes de repetir o ciclo
}
