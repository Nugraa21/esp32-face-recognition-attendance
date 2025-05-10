CREATE DATABASE absensi_db;

USE absensi_db;

-- Tabel absensi
CREATE TABLE absensi (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_name VARCHAR(50),
    tanggal DATE,
    jam TIME,
    status VARCHAR(20)
);

-- Tabel users (buat dataset)
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50),
    photo VARCHAR(100)
);
