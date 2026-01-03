#include "api_handler.h"
#include "certs.h"
#include "config.h"
#include "time_handler.h" // Added back for ntp/rtc
#include <HTTPClient.h>
#include <WiFi.h>

extern QueueHandle_t sensorQueue;

void setupWifi() {
  Serial.print("\n[WIFI] Menghubungkan ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Terhubung!");
}

void taskKirimAPI(void *parameter) {
  Serial.printf("Task API dimulai (interval: %d ms).\n", API_SEND_INTERVAL_MS);

  SensorData d;
  for (;;) {
    // Sinkronisasi NTP tetap berjalan di background untuk internal RTC
    if (ntp.update()) {
      rtc.setTime(ntp.getEpochTime());
    }

    // Ambil data dari Queue (tunggu sebentar jika kosong, tapi karena overwrite
    // harusnya selalu ada)
    if (xQueueReceive(sensorQueue, &d, 100)) {

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;

        // Gunakan sertifikat root CA untuk HTTPS
        http.begin(apiEndpoint, root_ca_cert);
        http.addHeader("Content-Type", "application/json");

        // Format JSON String (Sesuai Target Anda)
        String jsonData = "{";
        jsonData += "\"ph\":\"" + String(d.ph, 1) + "\",";
        jsonData += "\"ec\":\"" + String(d.ec, 0) + "\",";
        jsonData += "\"raindrop\":\"" + String(d.raindrop) +
                    "\","; // Kirim nilai raw (int) sebagai string
        jsonData += "\"tds\":\"" + String(d.tds, 0) + "\",";
        jsonData += "\"suhu\":\"" + String(d.temperature, 1) + "\",";
        jsonData += "\"condition\":\"" + d.acidStatus + "\",";
        jsonData += "\"created_at\":\"" + d.timestamp + "\"";
        jsonData += "}";

        // Tampilkan data yang akan dikirim untuk debug
        Serial.print("[API] Mengirim: ");
        Serial.println(jsonData);

        int httpCode = http.POST(jsonData);

        if (httpCode > 0) {
          Serial.printf("[API] Sukses (Code: %d) | Response: %s\n", httpCode,
                        http.getString().c_str());
        } else {
          Serial.printf("[API] Gagal (Error: %s)\n",
                        http.errorToString(httpCode).c_str());
        }

        http.end();
      } else {
        Serial.println("[API] Wifi terputus, mencoba reconnect...");
        WiFi.begin(ssid, password); // Coba reconnect ringan
      }
    }

    // Delay sesuai interval (misal 2 menit)
    vTaskDelay(API_SEND_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}