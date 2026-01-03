#include "raindrop_sensor.h"
#include "config.h"

int globalNilaiSensor = 0;
bool globalStateHujan = false;

void setupRaindrop() {
  pinMode(PIN_SENSOR_HUJAN, INPUT);
  Serial.println("Modul Sensor Hujan siap.");
}

void taskReadRaindrop(void *parameter) {
  Serial.printf("Task Sensor Hujan dimulai (interval: %d ms).\n",
                SENSOR_READ_INTERVAL_MS);

  for (;;) {
    int nilaiSekarang = analogRead(PIN_SENSOR_HUJAN);

    globalNilaiSensor = nilaiSekarang;

    Serial.print("[LOG] Nilai Sensor: ");
    Serial.print(nilaiSekarang);
    Serial.print(" | Status: ");
    Serial.println(globalStateHujan ? "HUJAN" : "KERING");

    if (nilaiSekarang < AMBANG_BATAS_HUJAN) {
      // Hujan terdeteksi
      if (globalStateHujan == false) {
        Serial.println("Sensor: Hujan terdeteksi!");
        globalStateHujan = true;
      }
    } else {
      // Kering
      if (globalStateHujan == true) {
        // Hanya update jika ada perubahan (dari hujan ke kering)
        Serial.println("Sensor: Kering terdeteksi!");
        globalStateHujan = false;
      }
    }
    vTaskDelay(SENSOR_READ_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}