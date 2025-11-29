#include <EEPROM.h>
#include <WiFi.h>
#include "config.h"
#include "time_handler.h"    
#include "sensor_temp.h"  
#include "sensor_ph.h"    
#include "sensor_tds.h"   
#include "sensor_ec.h"    
#include "fuzzy_logic.h" 
#include "api_handler.h" 

QueueHandle_t sensorQueue;

void TaskReadSensors(void *pvParameters) {
  (void) pvParameters;
  
  initTempSensor();
  initTDSSensor();
  initPHSensor();
  initECSensor();

  for (;;) {
    SensorData data;
    data.timestamp = get_date_time(0);
    data.temperature = readTempSensor();
    data.ph = readPHSensor(data.temperature);
    data.tds = readTDSSensor(data.temperature);
    data.ec = readECSensor(data.temperature);
    data.acidStatus = calculateFuzzyStatus(data.ph);

    // Kirim ke Queue
    xQueueOverwrite(sensorQueue, &data);

    // Delay 5 Detik
    vTaskDelay(5000 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(1024); 

  // 1. Setup WiFi (Menggunakan fungsi dari api_handler.h)
  setupWifi();

  // 2. Setup Waktu
  setupTime();

  // 3. Setup RTOS
  sensorQueue = xQueueCreate(1, sizeof(SensorData));

  // Task Sensor (Core 1)
  xTaskCreatePinnedToCore(TaskReadSensors, "SensorTask", 4096, NULL, 2, NULL, 1); 
  
  // Task API/Network (Core 0) -> Menggunakan fungsi dari api_handler.h
  xTaskCreatePinnedToCore(taskKirimAPI, "ApiTask", 8192, NULL, 1, NULL, 0); 
}

void loop() {
  vTaskDelete(NULL);
}