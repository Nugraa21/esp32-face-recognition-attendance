#include "esp_camera.h"
#include <WiFi.h>

// ===================
// Pilih model kamera
// ===================
#define CAMERA_MODEL_AI_THINKER // Has PSRAM

// Definisi pin AI Thinker ESP32-CAM:
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

// OPTIONAL: kalau mau kontrol LED flash
#define LED_GPIO_NUM     4

// ===========================
// Masukkan WiFi credentials
// ===========================
const char *ssid = "nugra";
const char *password = "081328400060";

void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // untuk streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Kalau ada PSRAM, pakai setting lebih tinggi
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    config.frame_size = FRAMESIZE_240X240;
  }

  // Inisialisasi kamera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // Setting tambahan buat AI Thinker (OV2640)
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(10000);  // biar nggak nge-freeze, server jalan di task lain
}

// =======================================================
// Tambahan: kamera server (pakai bawaan ESP32-CAM lib)
// =======================================================
#include <WebServer.h>

WebServer server(80);

void startCameraServer() {
  // Rute untuk halaman utama
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "ESP32-CAM OK! Buka /capture untuk foto.");
  });

  // Rute untuk menangkap gambar
  server.on("/capture", HTTP_GET, []() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      server.send(500, "text/plain", "Gagal ambil foto");
      return;
    }
    server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
    server.send_P(200, "image/jpeg", (char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
  });

  // Rute untuk status server
  server.on("/status", HTTP_GET, []() {
    String status = "ESP32-CAM Server\n";
    status += "IP Address: " + WiFi.localIP().toString() + "\n";
    status += "Camera Initialized: Yes (Camera is initialized during setup)\n";
    server.send(200, "text/plain", status);
  });

  server.begin();
}

// OPTIONAL: Setup LED flash
void setupLedFlash(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}
