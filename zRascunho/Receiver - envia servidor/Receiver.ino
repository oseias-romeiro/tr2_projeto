#include <LoRa.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Pinos e configuração do LoRa
const int CS_PIN = 10;
const int RESET_PIN = 9;
const int IRQ_PIN = 2;

// Configurações WiFi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// URL do servidor para onde enviar os dados
const char* serverUrl = "http://tr2.alwaysdata.net/tanque/2";

void setup() {
  Serial.begin(9600);

  // Inicializa LoRa
  LoRa.setPins(CS_PIN, RESET_PIN, IRQ_PIN);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Initializing OK!");

  // Conectando ao WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    String receivedData = "";
    while (LoRa.available()) {
      receivedData += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();

    // Exibir dados recebidos e RSSI no Serial Monitor
    Serial.print("Received packet '");
    Serial.print(receivedData);
    Serial.print("' with RSSI ");
    Serial.println(rssi);

    // Enviar dados para o servidor
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      // Construindo os dados do POST
      String postData = "distance=" + receivedData;

      int httpResponseCode = http.POST(postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Server Response: " + response);
      } else {
        Serial.println("Error sending data: " + String(httpResponseCode));
      }

      http.end();
    } else {
      Serial.println("WiFi not connected");
    }
  }
}
