from flask import Flask, render_template, request, redirect, url_for, flash
import sqlite3
import cv2
import numpy as np
import os
from datetime import datetime

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = 'dataset'
app.secret_key = 'secret'

# Ensure dataset folder exists
if not os.path.exists('dataset'):
    os.makedirs('dataset')

DATABASE = '../database/absensi.db'

def get_db_connection():
    conn = sqlite3.connect(DATABASE)
    conn.row_factory = sqlite3.Row
    return conn

# Home page: Tampilkan absensi
@app.route('/')
def index():
    conn = get_db_connection()
    absensi = conn.execute('SELECT * FROM absensi').fetchall()
    conn.close()
    return render_template('index.html', absensi=absensi)

# Upload & Face Recognition endpoint
@app.route('/upload', methods=['POST'])
def upload():
    file = request.data
    nparr = np.frombuffer(file, np.uint8)
    img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)
    
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    recognizer = cv2.face.LBPHFaceRecognizer_create()
    recognizer.read('trained_model.yml')

    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)
    
    conn = get_db_connection()

    for (x, y, w, h) in faces:
        id_, conf = recognizer.predict(gray[y:y+h, x:x+w])
        if conf < 70:
            name = conn.execute('SELECT nama FROM users WHERE id = ?', (id_,)).fetchone()
            if name:
                now = datetime.now()
                conn.execute('INSERT INTO absensi (user_id, nama, waktu) VALUES (?, ?, ?)',
                             (id_, name['nama'], now.strftime('%Y-%m-%d %H:%M:%S')))
                conn.commit()
                conn.close()
                return f"Absensi sukses untuk {name['nama']}"
    conn.close()
    return "Wajah tidak dikenali"

# CRUD: Tambah user
@app.route('/tambah', methods=('GET', 'POST'))
def tambah():
    if request.method == 'POST':
        nama = request.form['nama']
        if nama:
            conn = get_db_connection()
            conn.execute('INSERT INTO users (nama) VALUES (?)', (nama,))
            conn.commit()
            conn.close()
            flash('User ditambahkan!')
            return redirect(url_for('index'))
    return render_template('tambah.html')

# CRUD: Edit user
@app.route('/edit/<int:id>', methods=('GET', 'POST'))
def edit(id):
    conn = get_db_connection()
    user = conn.execute('SELECT * FROM users WHERE id = ?', (id,)).fetchone()
    if request.method == 'POST':
        nama = request.form['nama']
        conn.execute('UPDATE users SET nama = ? WHERE id = ?', (nama, id))
        conn.commit()
        conn.close()
        flash('User diupdate!')
        return redirect(url_for('index'))
    return render_template('edit.html', user=user)

# CRUD: Delete user
@app.route('/delete/<int:id>')
def delete(id):
    conn = get_db_connection()
    conn.execute('DELETE FROM users WHERE id = ?', (id,))
    conn.commit()
    conn.close()
    flash('User dihapus!')
    return redirect(url_for('index'))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
