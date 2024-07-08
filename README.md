# Projeto Final de TR2 - Monitoramento Remoto de Tanques de Combustível
Projeto de criação e implementação de um sistema de monitoramento remoto do nível de tanques de combustível utilizando comunicação LoRa e dispositivos Arduino para a disciplina de Teleinformática e Redes 2 na Universidade de Brasília (UnB) em 2024 (2024.1).

  - Felipe Fontenele dos Santos -  190027622
  - Oseias Romeiro Magalhães - 211036123
  - Paulo Victor França de Souza - 200042548

## Demonstração:
Video demonstrativo em ambiente de teste: [youtu.be/Vv9kiqzvt7s](https://youtu.be/Vv9kiqzvt7s)

## Setup 
Para rodar o projeto, deve-se instalar os seguintes requisitos:
  - [Arduino CLI](https://arduino.github.io/arduino-cli/0.34/installation)
  ```sh
    arduino-cli lib install "LoRa"
    arduino-cli lib install "Ultrasonic"
    arduino-cli lib install "Sleep_n0m1"
  ```
  - [Python](https://www.python.org/downloads/)
  ```sh
    pip install pyserial
    pip install requests
  ```

### Executando
Utilizar os seguintes comandos no terminal:

```sh
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

### Web Server
Executando servidor web localmente:

```sh
cd ./web
pip install -r requirements.txt
flask run
```

> Veja o serviço hospedado em [tr2.alway.net](https://tr2.alwaysdata.net/)

## Como o projeto funciona?
Cada tanque teria um arduino com um sensor de ultrassom acoplado que mede o nível de combustível no tanque e envia o dado via LoRa (tecnologia de rádio para longo alcançe com baixa energia) para um receptor central, que deve receber o dado e encaminhar para o um serviço web que cria um dashboard com base nos dados coletados.

Neste projeto, utilizaremos os termos *Node* para os arduinos que coleta e envia os dados do sensor e *Gateway* para o arduino que faz a função de receptor central.

- **_Node_**: Consiste em um Arduino Uno conectado a uma protoboard, que possui um sensor ultrassônico para medir o nível de combustível no tanque com base no tempo de reflexão das ondas, calcuando o quão vazio está. Além disso, ele é conectado a um LoRa Shield para transmitir os dados coletados pelo sensor.
  
  <img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/35950027-83a9-4eb5-b284-10f58f677879" width="300"/>


- **_Gateway_**: Consiste em um Arduino Uno conectado a um LoRa Shield para recebimento dos dados enviados por algum _Node_.
  
  <img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/7fa78a23-47a7-4ef7-a647-0fe80767ec0d" width="300"/>

### Comunicação *Gateway* e *Nodes*
A comunicação se inicia com o *gateway* enviando um sinal de broadcast para os *nodes*. Após isso, ele aguarda 10 segundos o sinal de algum *node* enviando o seu ID. Essa tentativa de conexão é realizada 3 vezes, a fim de garantir que mesmo com algumas perdas de pacotes ou dessincronização (com um certo limite) a conexão seja concluída com sucesso. Caso contrário, infere-se que não há nenhum *node* acordado, portanto ele entra em modo de baixo consumo de energia por um determinado tempo. Caso receba sinal de algum *node*, o gateway confirma para assim estabelecer a conexão entre eles.

Então o *node* captura os dados do sensor, faz a medição em **centímetros**, envia o dado para o *gateway* e entra em estado de baixo consumo de energia por um determinado tempo.

O *gateway* recebe os dados do *node*, insere em formato JSON juntamente com logs das etapas anteriores e escreve na portal serial para ser lidas pelo programa [SerialListner](./SerialListener.py), depois formatada e enviada para o servidor web.


## Correções e Aprimoramentos

Apesar de não ter sido implementada, podemos aprimorar o projeto adicionando uma placa wifi ao *gateway* para ele conectar-se diretamente ao servidor web.

Otutro ponto importante é a questão dos IDs que foram implementadas como contantes no código do *node*, porém a ideia é não haver necessidade de programação de cada um. Deve ser atribuido automaticamente ao conectar-se na rede, pois a princípio não se sabe a quantidade de *nodes* e deve poder ser inserido um a qualquer momento.

Além disso, um problema presente na implemtação atual é a dificuldade do *gateway* de se comunicar com os *nodes* em um ambiente muito concorrido com diversos dispositivos, o que será abordado logo abaixo.

### Solução para Problemas de Comunicação na Implemetnação
Quando o *gateway* tenta responder um *node*, a rede pode estar conturbada pois vários *nodes* estão ao mesmo tempo tentando enviar seu ID ao *gateway*. Neste momento nenhum nó consegue receber a resposta do *gateway* e assim nenhuma conexão será estabelecida, fazendo assim, com que todos os *nodes* fiquem esperando o *gateway*, enquanto que para ele, não há  nenhum *node* acordado.

Uma solução é impedir que todos os *nodes* tentem estabelecer conexão ao mesmo tempo, sendo necessário então o *gateway* dizer quando um *node* pode acordar e tentar comunicar-se, formando assim um unicast entre o *gateway* e o *node*. Para isso, o *gateway* deve ser capaz de:
  1. Reconhecer cada *node* que é inserido na rede e o atribuindo um ID.
  2. Designar momentos específicos para a comunicação de cada *node*.
  3. Remover *nodes* inativos da rede do controle.

Dessa forma, a comunicação entre o gateway e os *nodes* será mais eficiente, reduzindo interferências e garantindo que as respostas sejam recebidas corretamente.

## Conclusões
Apesar dos problemas abordados e das propostas de aprimoramentos em [Correções e Aprimoramentos](#correções-e-aprimoramentos), o projeto é funcional como mostrado em [video](#demonstração) com o ambiente de teste que tivemos acesso, aplpicando assim, na prática os conhecimentos da disciplina na implementação de uma solução para um problema real.

## Imagens

### *Gateway* e *Node*
<img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/a547a09c-f842-4edb-b5ee-e438849f7cfd" width="800"/>

<img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/a2f926a7-e9e3-4d89-9663-73d3ebc281db" width="800"/>

### Interface web do servidor
<img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/d4acb01a-ba33-471b-867d-027118ee195e" width="800"/>

<img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/eb87b6a7-2403-4e3a-adb3-ca50320a187e" width="800"/>

<img src="https://github.com/oseias-romeiro/tr2_projeto/assets/73205479/3ffde8cc-6acb-474d-8b4a-f6311d9ab410" width="800"/>

