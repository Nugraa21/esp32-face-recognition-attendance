from flask import Flask, request, jsonify
import face_recognition
import numpy as np
import cv2
import os
import mysql.connector
from datetime import datetime

app = Flask(__name__)

# Connect DB
db = mysql.connector.connect(
    host="localhost",
    user="root",
    password="",
    database="absensi_db"
)

# Load dataset dari database users
def load_known_faces():
    cursor = db.cursor()
    cursor.execute("SELECT * FROM users")
    users = cursor.fetchall()

    encodings = []
    names = []

    for user in users:
        name = user[1]
        photo = user[2]
        image_path = f"../dashboard/uploads/{photo}"
        if os.path.exists(image_path):
            image = face_recognition.load_image_file(image_path)
            encoding = face_recognition.face_encodings(image)
            if encoding:
                encodings.append(encoding[0])
                names.append(name)
    return encodings, names

known_face_encodings, known_face_names = load_known_faces()

@app.route('/upload', methods=['POST'])
def upload_image():
    file = request.files['image']
    file_path = f"../uploads/{file.filename}"
    file.save(file_path)

    img = cv2.imread(file_path)
    rgb_img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    face_locations = face_recognition.face_locations(rgb_img)
    face_encodings = face_recognition.face_encodings(rgb_img, face_locations)

    for face_encoding in face_encodings:
        matches = face_recognition.compare_faces(known_face_encodings, face_encoding)
        face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
        best_match_index = np.argmin(face_distances)
        if matches[best_match_index]:
            name = known_face_names[best_match_index]

            # Masukkan ke tabel absensi
            now = datetime.now()
            cursor = db.cursor()
            cursor.execute("INSERT INTO absensi (user_name, tanggal, jam, status) VALUES (%s, %s, %s, %s)",
                           (name, now.date(), now.time(), "Hadir"))
            db.commit()

            return jsonify({"message": f"Absensi berhasil untuk {name}"}), 200

    return jsonify({"message": "Wajah tidak dikenali"}), 400

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
