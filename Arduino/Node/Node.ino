#include <Ultrasonic.h>
#include <SPI.h>
#include <LoRa.h>

#define PIN_TRIG 3
#define PIN_ECHO 4

Ultrasonic ultrasonic(PIN_TRIG, PIN_ECHO);
const String TANQUE_ID = "1001";

// retorna falso ao receber sinal do gateway
bool waitingGatewaySignal() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return true;
  else return false;
}

// evnia o id como sinal de que está acordado
void sendSignal(){
  Serial.println("Enviando ID para o gateway");
  LoRa.beginPacket();
  LoRa.print(TANQUE_ID);
  LoRa.endPacket();
  Serial.println(TANQUE_ID);
}

//retorna true quando o gateway retornar um sinal igual a TANQUE_ID
bool satabilishConnection() {
  // obtem a resposta do gateway
  int packetSize = LoRa.parsePacket();
  if(!packetSize) return false;
  String resposta = "";
  while (LoRa.available()) {
    resposta += (char)LoRa.read();
  }
  // verifica se o gateway confirmou o id
  if(TANQUE_ID == resposta){
    Serial.println("gateway confirmou me enviando meu id");
    return true;
  }else {
    // espera a vez de outro
    // 3000 seria um tempo suficiente para o outro comunicar
    // mas nao longo o suficiente para causar um starvation
    delay(3000);
    // espera o gateway
    while (waitingGatewaySignal()){
      Serial.println("esperando o sinal do gateway");
      delay(100);
    }
  } 
  
}

// envia a distancia para o gateway
void sendDistance(long distance){
  LoRa.beginPacket();
  LoRa.print(String(distance));
  LoRa.endPacket();
  Serial.println("Dado enviado:");
  Serial.println(distance);
}

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);
}

void loop() {

  // espera o sinal do gateway
  while (waitingGatewaySignal()){
    Serial.println("esperando o sinal do gateway");
    delay(100);
  }
  // sinaliza que ta acordado usando o id
  // espera o gateway confirmar respondendo com o mesmo id
  while (!satabilishConnection()){ sendSignal(); }
  // faz a medição
  long distance = ultrasonic.read();
  // envia o dado
  sendDistance(distance);
  // dorme por 10 segundos
  delay(10000);
}