from flask import Flask
import os, psycopg2

HOST = os.getenv('HOST', '0.0.0.0')
PORT = os.getenv('PORT', 8080)
DB_HOST = os.getenv('DB_HOST', 'db')
DB_PORT = os.getenv('DB_PORT', 5432)
DB_USER = os.getenv('DB_USER', 'postgres')
DB_PASSWORD = os.getenv('DB_PASSWORD', 'postgres')
DB_NAME = os.getenv('DB_NAME', 'postgres')

app = Flask(__name__)

def connect():
    conn = psycopg2.connect(
        host=DB_HOST,
        database=DB_NAME,
        user=DB_USER,
        password=DB_PASSWORD
    )
    print("Database connected")

@app.route('/')
def index():
    return 'Projeto de TR2 para controle de nivel de combustiveis em tanques de armazenamento'

if __name__ == '__main__':
    connect()
    app.run(host=HOST, port=PORT)
