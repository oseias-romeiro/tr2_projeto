# url = "http://tr2.alwaysdata.net/tanque/2"
# data = {'distance': 28}

# response = requests.post(url, data=data)

# print(f"Status Code: {response.status_code}")
# print(f"Response: {response.text}")

# Abrindo o arquivo localmente
file_path = 'tr2_projeto/zRascunho/Receiver - salva em txt/datalog.txt'

# Lendo o conteúdo do arquivo
with open(file_path, 'r') as file:
    content = file.read()

# Encontrando o valor de "distance"
distance_index = content.find('distance: ')
if distance_index != -1:
    # Procurando o valor numérico após 'distance: '
    start_index = distance_index + len('distance: ')
    end_index = content.find('\n', start_index)
    distance_value = content[start_index:end_index].strip()

    # Convertendo para um número (se necessário)
    distance_value = float(distance_value)  # Converte para float, se o valor for decimal
    # distance_value = int(distance_value)  # Converte para inteiro, se o valor for inteiro

    print(f"Valor de 'distance' encontrado no arquivo: {distance_value}")
else:
    print("Não foi possível encontrar 'distance' no arquivo.")
