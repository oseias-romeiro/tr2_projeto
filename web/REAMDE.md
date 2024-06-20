# Aplicação web
Aplicação web para o sistema monitoramento de tanques de combustíveis

## Setup
```sh
pip install -r requirements.txt
flask db init
flask db migrate -m "init"
flask db upgrade
```

## Executar
```sh
flask run
```

## Endpoints
Rota            |Método|Descrição
---             | ---  | ---
/               |GET   |Dashboard de monitoramento
/tanque/\<id\>  |POST  |Cria o tanque se não existir e adiciona os dados dos sensores `{capacidade: <float>, nivel: <float>}`