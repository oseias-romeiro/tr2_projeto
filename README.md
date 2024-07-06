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
