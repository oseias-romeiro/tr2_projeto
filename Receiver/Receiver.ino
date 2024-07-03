#include <LoRa.h>

// Pinos e configuração do LoRa
const int CS_PIN = 10;
const int RESET_PIN = 9;
const int IRQ_PIN = 2;
const int ID_CONNECTED = 1;

unsigned long int timeToSleep = 1;
unsigned long int timeToWait = 1;
bool synced = false;


void setup() {
  Serial.begin(9600);

  // Inicializa LoRa
  LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }

  Serial.println("LoRa Initializing OK!");
}

int Sync(){
    String receivedData = "";
    int packetSize = 0;

    Serial.println("Aguardando primeiro pacote.");
    while (!receivedData) {
      receivedData = ReceiveData();;
      delay(1000);
    }

    SendDataToSerial(receivedData);    

    receivedData = "";
    int beginTime = millis();
    
    Serial.println("Aguardando segundo pacote.");
    while (!receivedData) {
      receivedData = ReceiveData();
      delay(1000);
    }

    SendDataToSerial(receivedData);   

    return millis() - beginTime;
}

void loop() {
  long int contador = 0;
  String receivedData = "";

  if (!synced) {
    Serial.println("Iniciando sincronização");

    int packetDelayTime = Sync();
    int baseDiffTime = packetDelayTime / 5;
    
    timeToSleep = packetDelayTime - baseDiffTime;
    timeToWait = packetDelayTime + baseDiffTime;

    Serial.print("Sincronização feita com sucesso. Tempo de intervalo: ");
    Serial.println(timeToSleep);
  } else {
    int packetSize = 0;
    long int endTime = millis() + timeToWait;

    while (millis() < endTime) {
      receivedData = ReceiveData();
      if (receivedData) {
        break;
      }
    }

    if (!receivedData) {
      Serial.print("Não foi recebido nenhum pacote no tempo de ");
      Serial.print(timeToSleep);
      Serial.println(" estipulado. Sincronização será realizada novamente.");

      synced = false;
      timeToSleep = 1;
    }
  }

  delay(timeToSleep);
}

void SendDataToSerial(String distance){
  Serial.println("{\"id\": \"" + String(ID_CONNECTED) + "\", \"distance\": \"" + distance + "\"}");
}

String ReceiveData() {
  String receivedData = "";
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }
  }

  return receivedData;
}
