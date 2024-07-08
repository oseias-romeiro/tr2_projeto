# Projeto de TR2
Projeto de criação e implementação de um sistema de monitoramento remoto do nível de tanques de combustível utilizando comunicação LoRa e dispositivos Arduino para a disciplina de Teleinformática e Redes 2 na Universidade de Brasília (UnB) em 2024 (2024.1).

## Setup 
Para rodar o projeto, deve-se instalar os seguintes requisitos:
  - https://arduino.github.io/arduino-cli/0.34/installation
  - ```
    arduino-cli lib install "LoRa"
    arduino-cli lib install "Ultrasonic"
    arduino-cli lib install "Sleep_n0m1"
    ```
  - https://www.python.org/downloads/
  - ```
    pip install pyserial
    pip install requests
    ```
Em seguida, deve-se utilizar os seguintes comandos no terminal:
  - ```
    arduino-cli compile --fqbn arduino:avr:uno "%~dp0\Node\Node.ino"
    arduino-cli upload -p <porta_usb_arduino_node> --fqbn arduino:avr:uno "%~dp0\Node\Node.ino"
    arduino-cli compile --fqbn arduino:avr:uno "%~dp0\Gateway\Gateway.ino"
    arduino-cli upload -p <porta_usb_arduino_gateway> --fqbn arduino:avr:uno "%~dp0\Gateway\Gateway.ino"
    start cmd /k arduino-cli monitor -p "%~dp0\Node\Node.ino"
    python SerialListener.py
    ```
OU
  - Windows: ``` Setup.bat ```
  - Linux: ``` chmod +x ./Setup.sh && ./Setup.sh ```

## Web Server
```
cd ./web
pip install -r requirements.txt
python3 app.py
```

## Como o projeto funciona?
A comunicação do sensor ultrassônico do tanque de combustível é realizada através de dois LoRa em conjunto com dispositivos Arduino. O LoRa que envia os dados do sensor foi denominado *Node* e o LoRa que espera a comunicação do *Node*, faz a conexão e recebe os dados do nível do tanque é o *Gateway*.

  - **_Node_**: Consiste em um Arduino Uno conectado a uma protoboard, que possui um sensor ultrassônico para medir o nível de combustível no tanque, conectado também a um LoRa Shield para transmitir os dados coletados pelo sensor.
    - <img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/35950027-83a9-4eb5-b284-10f58f677879" width="300"/>
    
   - **Gateway_**: Consiste em um Arduino Uno conectado a um LoRa Shield para recebimento dos dados enviados pelo _Node_.
     - <img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/7fa78a23-47a7-4ef7-a647-0fe80767ec0d" width="300"/> 

De tempos em tempos, o dado do sensor é lido, enviado através do *Node* para o *Gateway* e então o *Gateway* formata os dados obtidos em formato JSON e os envia para a portal serial, a qual é escutada pelo serviço *SerialListener*, responsável por fazer um post do JSON no servidor https://tr2.alwaysdata.net, no qual é registrado os dados em um banco de dados.

### Comunicação *Gateway* e *Node*
A comunicação se inicia com o *gateway* enviando um sinal de broadcast. Após isso, ele aguarda 10 segundos o sinal de algum *Node* enviando o seu ID. Essa tentativa de conexão é realizada 3 vezes, então temos 30 segundos totais de espera.

Com o ID do *node*, é feito uma confirmação com o mesmo, onde o *gateway* envia o ID recebido de volta para o *node* e aguarda o registro do sensor.

Com tudo isso feito com sucesso, é escrito um JSON na portal serial com os dados obtidos no ciclo.
