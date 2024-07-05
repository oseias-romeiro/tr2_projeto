from flask import Flask, render_template, request, redirect, url_for
from flask_sqlalchemy import SQLAlchemy
from flask_migrate import Migrate
import os
from datetime import datetime
from collections import deque

DB_URI = os.getenv('DB_URI', 'sqlite:///db.sqlite3')

# setup
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = DB_URI
db = SQLAlchemy(app)
migrate = Migrate(app, db)

# models
class Tanque(db.Model):
    __tablename__ = 'tanque'
    id = db.Column(db.Integer, primary_key=True)
    capacidade = db.Column(db.Float, nullable=False, default=100.0)
    descricao = db.Column(db.String(100), nullable=True)

    def __repr__(self):
        return f'<Tanque {self.nome}>'

class SensorData(db.Model):
    __tablename__ = 'sensor_data'
    id = db.Column(db.Integer, primary_key=True)
    datetime = db.Column(db.DateTime, nullable=False)
    nivel = db.Column(db.Float, nullable=False)
    status = db.Column(db.String(20), nullable=False)
    id_tanque = db.Column(db.Integer, db.ForeignKey('tanque.id'), nullable=False)

    tanque = db.relationship('Tanque', backref=db.backref('sensor_datas', lazy=True))

    def __repr__(self):
        return f'<SensorData {self.datetime}>'

# routes
@app.route('/')
def index():
    tanques = Tanque.query.all()
    for t in tanques:
        t.data = (
            SensorData.query
            .filter_by(id_tanque=t.id)
            .order_by(SensorData.datetime.desc())
            .limit(1)
            .all()
        )
        # transforma datetime em string
        for d in t.data:
            d.datetimestr = d.datetime.strftime('%d/%m %H:%M:%S')
            d.nivel = round(d.nivel, 2)
            
    msg = request.cookies.get('msg', '')
    return render_template('index.html', tanques=tanques, msg=msg)

@app.route('/tanque/<int:id>', methods=['GET', 'POST'])
def tanque(id):
    if request.method == 'POST':
        if(not id): return "Espera-se um id em /tanque/<id>", 304
        try:
            tanque = Tanque.query.get(id)
            if not tanque:
                tanque = Tanque(id=id)
                db.session.add(tanque)
                db.session.commit()
            sensor_data = SensorData(
                datetime=datetime.strptime(request.form['datetime'], '%Y-%m-%d %H:%M:%S'),
                nivel=100-float(request.form['distance']),
                status="", # TODO: Implementar lógica para definir o status
                id_tanque=id
            )
            db.session.add(sensor_data)
            db.session.commit()

            return "Dados inseridos com sucesso", 200
        except Exception as e:
            print(str(e))
            return str(e), 500
    elif request.method == 'GET':
        tanque = Tanque.query.get(id)
        data = (
            SensorData.query
            .filter_by(id_tanque=id)
            .order_by(SensorData.datetime.asc())
            .all()
        )
        # variação da última hora
        niveis1h = [d.nivel for d in data if (datetime.now() - d.datetime).total_seconds() < 3600]
        if niveis1h:
            tanque.variacao = round(max(niveis1h) - min(niveis1h), 2)
        else:
            tanque.variacao = 0
        # tempo estimado de vida
        if data and tanque.variacao > 0:
            tanque.tempo = round(data[-1].nivel / tanque.variacao, 2)
        else:
            tanque.tempo = 0
        # transforma datetime em string
        for d in data:
            d.datetimestr = d.datetime.strftime('%d/%m %H:%M:%S')
        if not tanque:
            return "Tanque não encontrado", 404
        return render_template('tanque.html', tanque=tanque, data=data)

@app.route('/tanque/logs', methods=['GET', 'POST'])
def tanqueLogs():
    if request.method == 'POST':
        try:
            logs = request.form['logs']
            data = request.form['data']
            date = request.form['datetime']
            f = open(f'logs.txt', 'a+', encoding='utf-8')
            f.write(f"[{date}]\n{data}\n")
            for l in logs.split('\n'): f.write(f"{l}\n")
            f.close()
            return "Logs inseridos com sucesso", 200
        except Exception as e:
            print(str(e))
            return str(e), 500
    elif request.method == 'GET':
        f = open(f'logs.txt', 'r', encoding='utf-8')
        logs = deque(f.readlines(), maxlen=1000)
        f.close()
        return render_template('logs.html', logs=logs)

@app.route('/tanque/<int:id>/edit', methods=['GET', 'POST'])
def editTanque(id):
    if request.method == 'POST':
        if(not id):
            return "Espera-se um id em /tanque/<id>/edit", 304
        try:
            tanque = Tanque.query.get(id)
            if not tanque:
                return "Tanque não encontrado", 404
            tanque.capacidade = float(request.form['capacidade'])
            tanque.descricao = request.form['descricao']
            db.session.commit()
            
            return render_template('msg.html', msg="Tanque editado com sucesso")
        except Exception as e:
            return render_template('msg.html', msg=str(e)), 500
    
    elif request.method == 'GET':
        tanque = Tanque.query.get(id)
        return render_template('edit_tanque.html', tanque=tanque)
