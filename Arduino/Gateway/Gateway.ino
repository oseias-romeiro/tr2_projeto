#include <SPI.h>
#include <LoRa.h>

String id_connected = "";
String data_recieved = "";

// retorna falso quando receber sinal de algum no
bool waitingNodeID() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;
  while (LoRa.available()) {
    id_connected += (char)LoRa.read();
  }
  return false;
}
// envia sinal broadcast
void sendBroadcastSignal() {
  LoRa.beginPacket();
  LoRa.print("broadcasting...");
  LoRa.endPacket();
}
// envia o sinal para o nó permitindo
void sendSignal2Node(){
  LoRa.beginPacket();
  LoRa.print(id_connected);
  LoRa.endPacket();
}
// coleta os dados do sensor
bool waitingNodeData() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;
  while (LoRa.available()) {
    data_recieved += (char)LoRa.read();
  }
  return false;
}

void sendData(){
  Serial.println("{id: '"+ id_connected +"', distance: '" + data_recieved + "'}");
}

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
}

void loop() {

  // manda um broadcast e recebe o id do nó
  int c = 0;
  while (waitingNodeID()){
    // se não receber sinal de nenhum nó em 3 tentativas, dorme por 10 segundos
    if(c >= 3) {
      delay(10000);
      c = 0; // reseta contador
    };
    // envia sinal de broadcast
    sendBroadcastSignal();
    delay(1000);
    c++;
  }
  // sinaliza o nó e recebe os dados
  while (waitingNodeData()){ sendSignal2Node(); }
  // envia o dado via serial
  sendData();
  // clear
  id_connected = "";
  data_recieved = "";
}