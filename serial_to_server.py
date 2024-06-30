# pip install pyserial
# pip install requests

import serial
import requests

def send_data_to_server(tank_id, nivel):
    url = f"https://tr2.alwaysdata.net/tanque/{tank_id}"  # Caminho atualizado
    payload = {'distance': nivel}  # Adicione a capacidade e o nível
    headers = {'Content-Type': 'application/x-www-form-urlencoded'}

    try:
        response = requests.post(url, data=payload, headers=headers)
        if response.status_code == 200:
            print("Data sent successfully")
        else:
            print(f"Failed to send data. Status code: {response.status_code}")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    try:
        ser = serial.Serial('COM6', 9600, timeout=1)
    except serial.SerialException as e:
        print(f"Could not open port: {e}")
        return

    tank_id = 1  # Substitua pelo ID do tanque correto
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Received: {data}")
            try:
                nivel = float(data)
                send_data_to_server(tank_id, nivel)
            except ValueError:
                print("Received invalid data")

if __name__ == "__main__":
    main()
