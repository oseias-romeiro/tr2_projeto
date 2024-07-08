# url = "http://tr2.alwaysdata.net/tanque/2"
# data = {'distance': 28}

# response = requests.post(url, data=data)

# print(f"Status Code: {response.status_code}")
# print(f"Response: {response.text}")

import sys
import serial # pip install pyserial
import requests # pip install requests
import json
from datetime import datetime

# Configurações da porta serial
if len(sys.argv) != 2:
    port = input("Digite a porta do Gateway (exemplo: COM5): ")
else:
    port = sys.argv[1]
baudrate = 9600
timeout = 1

# URL para enviar os dados
url = "http://tr2.alwaysdata.net"

# Inicializa a conexão serial
ser = serial.Serial(port, baudrate, timeout=timeout)

# Função para enviar dados via POST request
def send_data(id, distance):
    data = {'distance': distance, 'datetime': datetime.now().strftime('%Y-%m-%d %H:%M:%S')}
    response = requests.post(url+f'/tanque/{id}', data=data)
    print(f"Status Code: {response.status_code}")
    print(f"Response: {response.text}")

def send_logs(logs, data):
    data = {'logs': logs, 'datetime': datetime.now().strftime('%Y-%m-%d %H:%M:%S'), 'data': data}
    response = requests.post(url+"/gateway/logs", data=data)
    print(f"Status Code: {response.status_code}")
    print(f"Response: {response.text}")

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(f"Received from serial: {line}")
            # Supondo que o dado recebido é um número representando a distância
            print(line)
            data = json.loads(line)
            if(data["id"] != ""):
                send_data(data["id"], data["nivel"])
            send_logs(data["logs"], json.dumps(data))
