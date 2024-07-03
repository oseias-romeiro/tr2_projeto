# url = "http://tr2.alwaysdata.net/tanque/2"
# data = {'distance': 28}

# response = requests.post(url, data=data)

# print(f"Status Code: {response.status_code}")
# print(f"Response: {response.text}")

import serial
import requests


# Configurações da porta serial
port = 'COM5'
baudrate = 9600
timeout = 1

# URL para enviar os dados
url = "http://tr2.alwaysdata.net/tanque/"

# Inicializa a conexão serial
ser = serial.Serial(port, baudrate, timeout=timeout)

# Função para enviar dados via POST request
def send_data(id, distance):
    data = {'distance': distance}
    response = requests.post(url+id, data=data)
    print(f"Status Code: {response.status_code}")
    print(f"Response: {response.text}")

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(f"Received from serial: {line}")
            # Supondo que o dado recebido é um número representando a distância
            data = line.split(",")
            send_data(data[0], data[1])
            
