#include "esp_camera.h"
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "nugra";
const char* password = "xxxxxxxx";

// Server IP (ubah sesuai IP laptop kamu)
String serverName = "http://192.168.xxx.xxx:5000/upload";

// Kamera Model AI Thinker
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

void setup() {
  Serial.begin(115200);
  
  // Connect WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");

  // Inisialisasi Kamera
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
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed: 0x%x", err);
    return;
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "image/jpeg");
      int httpResponseCode = http.POST(fb->buf, fb->len);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(response);
      } else {
        Serial.printf("Failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }
      http.end();
      esp_camera_fb_return(fb);
    }
  }
  delay(10000);  // Ambil foto tiap 10 detik
}
