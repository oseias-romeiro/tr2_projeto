#!/bin/bash

# Requisitos de instalação:
# arduino-cli lib install "LoRa"
# arduino-cli lib install "Ultrasonic"
# arduino-cli lib install "Sleep_n0m1"

# ------------------------------------------------------

echo ""
echo "#----------------#"
echo "# Portas Serial: #"
echo "#----------------#"
arduino-cli board list

# Solicita a porta NODE ao usuário
read -p "Digite a porta do Node (exemplo: /dev/ttyUSB0): " NODE_PORT

# Solicita a porta GATEWAY ao usuário
read -p "Digite a porta Gateway (exemplo: /dev/ttyUSB1): " GATEWAY_PORT

# Caminho para o arquivo Node
NODE_PATH="$(dirname "$0")/Node/Node.ino"

# Caminho para o arquivo Gateway
GATEWAY_PATH="$(dirname "$0")/Gateway/Gateway.ino"

# ------------------------------------------------------

echo ""
echo "---------------------------------------------------------------------------------------"
echo "NODE"
echo "---------------------------------------------------------------------------------------"
echo ""

echo "-----------"
echo "- COMPILE -"
echo "-----------"
echo ""

arduino-cli compile --fqbn arduino:avr:uno "$NODE_PATH"

echo ""
echo "----------"
echo "- UPLOAD -"
echo "----------"
echo ""

arduino-cli upload -p "$NODE_PORT" --fqbn arduino:avr:uno "$NODE_PATH"

echo ""
echo "---------------------------------------------------------------------------------------"
echo "GATEWAY"
echo "---------------------------------------------------------------------------------------"
echo ""

echo ""
echo "-----------"
echo "- COMPILE -"
echo "-----------"
echo ""

arduino-cli compile --fqbn arduino:avr:uno "$GATEWAY_PATH"

echo ""
echo "----------"
echo "- UPLOAD -"
echo "----------"
echo ""

arduino-cli upload -p "$GATEWAY_PORT" --fqbn arduino:avr:uno "$GATEWAY_PATH"

echo ""
echo "---------------------------------------------------------------------------------------"
echo "SERIAL LISTENER"
echo "---------------------------------------------------------------------------------------"
echo ""

gnome-terminal -- bash -c "echo -- Node -- && echo && arduino-cli monitor -p $NODE_PORT"
echo "-- Gateway -- && echo"
python3 SerialListener.py "$GATEWAY_PORT"

echo ""
echo "---------------------------------------------------------------------------------------"
echo ""

pause