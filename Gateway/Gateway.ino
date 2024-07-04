#include <SPI.h>
#include <LoRa.h>
#include <Sleep_n0m1.h>

// COM5 (ESQUERDA)

Sleep sp;
const int SLEEP_TIME = SLEEP_TIME; // 30 segundos
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
  LoRa.print("broadcast");
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
void sendData(String logs) {
  Serial.println("{\"id\":\"" + id_connected + "\",\"nivel\":\"" + data_received + "\",\"logs\":\"" + logs + "\"}");
}

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
  pinMode(12, INPUT);
}

void loop() {
  // Envia broadcast e aguarda o ID do nó
  bool deadNodes = false;
  String logs = "";
  int c = 0;
  while(true){
    if(debug) Serial.println("Enviado sinal de broadcast");
    logs += "Enviado sinal de broadcast\n";
    sendBroadcastSignal();
    delay(1000);
    // Espera a resposta de um nó
    int timeout = 5000; // Tempo máximo de espera (10 segundos)
    unsigned long start = millis();
    bool ok = false;
    while(millis() - start < timeout){
      if(!waitingNodeID()){
        ok = true;
        if(debug) Serial.println("Id recebido: " + id_connected);
        logs += "Id recebido: " + id_connected + "\n";
        logs += "Tentativa " + String(c+1) + "\n";
        break;
      };
    }
    if(ok) break;
    c++;
    // 3 tentativas para receber o ID
    if(c > 3) {
      deadNodes = true;
      logs += "! nós não respondem !\n";
      break;
    }
  }
  if(!deadNodes){
    // Aguarda sinal do nó e recebe os dados
    delay(2000);
    int c = 0;
    bool connectionFailed = false;
    while (true) {
      if(debug) Serial.println("Conectando com o nó " + id_connected);
      logs += "Conectando com o nó " + id_connected + "\n";
      establishConnection();
      delay(1000);
      // Aguarda os dados do nó
      int timeout = 5000; // Tempo máximo de espera (10 segundos)
      unsigned long start = millis();
      bool ok;
      while(millis() - start < timeout){
        if(!waitingNodeData()) {
          ok = true;
          if(debug) Serial.println("Dados recebidos do nó: " + data_received);
          logs += "Dados recebidos do nó: " + data_received + "\n";
          logs += "Tentativa " + String(c+1) + "\n";
          break;
        }
      }
      if(ok) break;
      if(debug) Serial.println("Falha, tentando novamente...");
      logs += "Falha, tentando novamente...\n";
      c++;
      // 3 tentativas para receber os dados
      if(c > 3) {
        connectionFailed = true;
        logs += "! nó não devolveu os dados !\n";
        break;
      }
    }
    if(!connectionFailed) {
      // Envia dados via serial
      logs += "Dormindo...\n";
      sendData(logs);

      // Limpa variáveis para o próximo ciclo
      id_connected = "";
      data_received = "";

      if(debug) Serial.println("Dormindo...");
      sp.pwrDownMode();
      sp.sleepDelay(SLEEP_TIME);
    }else {
      if(debug) Serial.println("Falha na conexão com o nó "+id_connected);
      logs += "Desconectando do nó " + id_connected + "\n";
    }
  }else {
    if(debug) Serial.println("Dormindo...");
    logs += "Dormindo...\n";

    // Envia dados via serial
    sendData(logs);

    // Limpa variáveis para o próximo ciclo
    id_connected = "";
    data_received = "";
    
    sp.pwrDownMode();
    sp.sleepDelay(SLEEP_TIME);
  }
}
