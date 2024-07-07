# Projeto de TR2
Projeto de Teleinformática de Redes 2 para controle de nível combustiveis em tanque de armazenamento

## Requisitos 
```
arduino-cli lib install "LoRa"
arduino-cli lib install "Ultrasonic"
arduino-cli lib install "Sleep_n0m1"
pip install pyserial
pip install requests
```

## Setup
Para rodar todo o projeto, deve-se usar os seguintes comandos no terminal (Windows):
```
Setup.bat
```
OU
```
arduino-cli compile --fqbn arduino:avr:uno "%~dp0\Node\Node.ino"
arduino-cli upload -p <porta_usb_arduino_node> --fqbn arduino:avr:uno "%~dp0\Node\Node.ino"
arduino-cli compile --fqbn arduino:avr:uno "%~dp0\Gateway\Gateway.ino"
arduino-cli upload -p <porta_usb_arduino_gateway> --fqbn arduino:avr:uno "%~dp0\Gateway\Gateway.ino"
start cmd /k arduino-cli monitor -p "%~dp0\Node\Node.ino"
python SerialListener.py
```

## Web Server
```
cd ./web
pip install -r requirements.txt
python3 app.py
```

## Como o projeto funciona?
A comunicação do sensor do tanque é realizada através de dois LoRa's.
O LoRa que envia os dados do sensor foi denominado *Node* e o LoRa que espera a comunicação do *Node* para enviar os dados do nível de água do tanque é o *Gateway*.

De tempos em tempos, o dado do sensor é lido, enviado através do LoRa do *Node* para o *Gateway* e então o *Gateway* formata os dados obtidos em formato JSON e os envia para a portal serial, a qual é escutada pelo serviço *SerialListener*. Esse serviço apenas pega o JSON e faz um post para registrar o dado no banco de dados.

### Comunicação *Gateway* e *Node*
A comunicação se inicia com o *gateway* enviando um sinal de broadcast. Após isso, ele aguarda 10 segundos o sinal de algum *Node* enviando o seu ID. Essa tentativa de conexão é realizada 3 vezes, então temos 30 segundos totais de espera.

Com o ID do *node*, é feito uma confirmação com o mesmo, onde o *gateway* envia o ID recebido de volta para o *node* e aguarda o registro do sensor.

Com tudo isso feito com sucesso, é escrito um JSON na portal serial com os dados obtidos no ciclo.
