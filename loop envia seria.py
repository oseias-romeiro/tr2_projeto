import serial
import time

# Configura a porta serial
ser = serial.Serial('COM6', 9600)  # Substitua '9600' pela taxa de baud correta

try:
    while True:
        # Dados a serem enviados
        data = 'Hello, world!'
        
        # Envia dados
        ser.write(data.encode('utf-8'))
        
        # Espera 1 segundo antes de enviar novamente
        time.sleep(1)

except KeyboardInterrupt:
    # Interrompe o loop quando o usuário pressionar Ctrl+C
    print("\nInterrompido pelo usuário")

finally:
    # Fecha a conexão serial
    ser.close()
    print("Conexão serial fechada")
