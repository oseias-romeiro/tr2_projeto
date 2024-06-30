import serial
import requests
import json

PORT = 'COM6'
HOST = 'tr2.alwaysdata.net'

def send_data_to_server(tank_id: str, distance: float):
    url = f"https://{HOST}/tanque/{tank_id}"  # Caminho atualizado
    payload = {'distance': distance}  # adiciona a distancia coletada
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
        ser = serial.Serial(PORT, 9600, timeout=1)
    except serial.SerialException as e:
        print(f"Could not open port: {e}")
        return

    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            data = json.loads(data)
            print(f"Received: {data}")
            try:
                send_data_to_server(data['id'], float(data['distance']))
            except ValueError:
                print("Received invalid data")

if __name__ == "__main__":
    main()
