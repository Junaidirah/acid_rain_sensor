#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <WiFi.h>
#include <HTTPClient.h>
#include "config.h"
#include "certs.h"
#include "time_handler.h"

// Kita butuh akses ke Queue dari main.ino
extern QueueHandle_t sensorQueue; 

void setupWifi() {
  Serial.print("Menyambungkan ke WiFi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung!");
  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());
}

void taskKirimAPI(void *parameter) {
  Serial.printf("Task API dimulai (interval: %d ms).\n", API_SEND_INTERVAL_MS);
  
  for (;;) {
    // 1. Update Waktu (Sesuai request Anda)
    if(ntp.update()) {
      rtc.setTime(ntp.getEpochTime());
      Serial.println("[API Task] Sinkronisasi ulang NTP sukses.");
    }

    // 2. Ambil Data dari Queue (Agar aman antar Task)
    SensorData dataToSend;
    bool dataReady = false;
    
    // Cek apakah ada data baru di Queue
    if (xQueuePeek(sensorQueue, &dataToSend, (TickType_t)10) == pdPASS) {
        dataReady = true;
    }

    // 3. Proses Pengiriman jika WiFi Connect dan Data Ada
    if (WiFi.status() == WL_CONNECTED && dataReady) {
      HTTPClient http;

      // Setup HTTPS dengan Sertifikat
      http.begin(apiEndpoint, root_ca_cert); 
      http.addHeader("Content-Type", "application/json");

      // Format Data Manual (JSON String Construction)
      // "seperti ini" sesuai permintaan user
      String jsonData = "{\"timestamp\":\"" + dataToSend.timestamp + "\"" + 
                        ", \"ph\":" + String(dataToSend.ph, 2) + 
                        ", \"tds\":" + String(dataToSend.tds, 0) + 
                        ", \"ec\":" + String(dataToSend.ec, 2) + 
                        ", \"temperature\":" + String(dataToSend.temperature, 1) + 
                        ", \"status\":\"" + dataToSend.acidStatus + "\"}";
      
      // --- Pengiriman Data ---
      Serial.println("[API Task] Mengirim data...");
      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.printf("[API Task] POST sukses (Code: %d), Data: %s\n", httpResponseCode, jsonData.c_str());
        Serial.println("Response: " + http.getString());
      } else {
        Serial.printf("[API Task] POST gagal, error: %s\n", http.errorToString(httpResponseCode).c_str());
      }

      http.end();
      
    } else {
      if (WiFi.status() != WL_CONNECTED) {
          Serial.println("[API Task] Wi-Fi terputus. Mencoba konek ulang...");
          WiFi.reconnect(); // Gunakan reconnect bawaan ESP32 yang lebih efisien
      } else {
          Serial.println("[API Task] Menunggu data sensor siap...");
      }
    }

    // Delay Task Sesuai Config
    vTaskDelay(API_SEND_INTERVAL_MS / portTICK_PERIOD_MS); 
  }
}

#endif