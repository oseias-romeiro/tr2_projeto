from flask import Flask
import os

HOST = os.getenv('HOST', '0.0.0.0')
PORT = os.getenv('PORT', 8080)

app = Flask(__name__)

@app.route('/')
def index():
    return 'Projeto de TR2 para controle de nivel de combustiveis em tanques de armazenamento'

if __name__ == '__main__':
    app.run(host=HOST, port=PORT)
