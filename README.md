# 📷 Proyek Absensi Wajah ESP32 + Python + Dashboard CRUD

Halo! Ini adalah proyek **Absensi Berbasis Pengenalan Wajah** menggunakan:

- 📡 **ESP32-CAM** untuk mengambil gambar & kirim ke server.
- 🐍 **Python Flask** untuk backend server + face recognition.
- 🌐 **Dashboard CRUD** (Create, Read, Update, Delete) berbasis web untuk kelola data & lihat laporan absensi.

---

## 🛠️ Fitur:

✅ Absensi otomatis via pengenalan wajah.  
✅ ESP32-CAM mengambil foto & upload ke server Python.  
✅ Server mengenali wajah, mencatat absensi secara real-time.  
✅ Dashboard web untuk:
- Lihat data absensi.
- Tambah user baru.
- Edit & hapus user.

---

## 📁 Struktur Folder:

```
absensi-face/
├── esp32/
│ └── esp32_face_cam.ino
├── server/
│ ├── app.py
│ ├── dataset/
│ │ └── (foto-foto wajah)
│ ├── trained_model.yml
│ ├── templates/
│ │ ├── index.html
│ │ ├── tambah.html
│ │ └── edit.html
│ └── static/
│ └── (CSS/JS jika ada)
└── database/
└── absensi.db
```


---

## ⚙️ Persiapan & Instalasi:

### 1️⃣ Siapkan ESP32-CAM

- Upload kode `esp32/esp32_face_cam.ino` pakai Arduino IDE.
- **Ubah bagian ini sesuai WiFi & server:**

```cpp
const char* ssid = "nugra";         // Ganti dengan WiFi kamu
const char* password = "081328400060";  // Password WiFi
String serverName = "http://192.168.225.136:5000/upload";  // IP laptop kamu

```
## Cek IP laptop kamu:

- Windows: ipconfig

- Linux: ifconfig

### 2️⃣ Siapkan Python Server
Masuk ke folder server/.

- Install dependencies:
```
pip install flask opencv-python opencv-contrib-python numpy

```
### 3️⃣ Buat Database SQLite
Buat file database/absensi.db.

Jalankan SQL berikut:
```sql
CREATE TABLE users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    nama TEXT NOT NULL
);

CREATE TABLE absensi (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_id INTEGER,
    nama TEXT,
    waktu TEXT
);
```
Bisa pakai DB Browser for SQLite untuk lebih mudah buat tabelnya.
### 4️⃣ Siapkan Face Recognition Model
- Pastikan kamu sudah punya dataset foto di server/dataset/ dengan struktur:

```
dataset/
  user1/
    1.jpg
    2.jpg
  user2/
    1.jpg
    2.jpg

```
- Latih model pakai script training (BUAT SENDIRI atau tambah nanti).

- Setelah latih, file trained_model.yml akan otomatis terbuat.

### 🚀 Jalankan Server:
Masuk ke folder server/ lalu jalanin:
```
python app.py

```
Server akan running di:
```
http://192.168.xxx.xxx:5000/
```
### 🖥️ Akses Dashboard:
- 🌐 Index: Lihat data absensi
>http://IP_LAPTOP:5000/

- ➕ Tambah User:
>http://IP_LAPTOP:5000/tambah

- ✏️ Edit User:
>Klik tombol edit dari dashboard.

### 🚨 Penting:
-  ESP32 & Laptop HARUS terhubung di jaringan WiFi yang sama.

- Pastikan firewall laptop tidak memblokir Flask server.

- IP ESP32 & Server harus benar biar komunikasi jalan.

## 🔥 Selesai! Selamat mencoba 🚀


---

✅ **Sudah lengkap banget ini README-nya**, Ludang. Ada langkah ESP32, Python, database, sampai lisensi ✔️.  
Mau tambah screenshot atau badge di atasnya juga? 😄
