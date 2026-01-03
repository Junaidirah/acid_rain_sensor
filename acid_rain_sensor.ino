#include "api_handler.h"
#include "config.h"
#include "fuzzy_logic.h"
#include "sensor_ec.h"
#include "sensor_ph.h"
#include "sensor_tds.h"
#include "sensor_temp.h"
#include "time_handler.h"
#include <EEPROM.h>
#include <WiFi.h>

const char *ssid = "A56";
const char *password = "banyak123";
const char *apiEndpoint = "https://biru-langit.com/api/v1/acid-rain/"
                          "50972188-be6b-43b8-9ce4-5d502e0a7e60";

QueueHandle_t sensorQueue;

void tampilkanLog(SensorData d) {
  Serial.println("\n--- DATA SENSOR ---");
  Serial.printf("Suhu: %.1f | pH: %.1f | TDS: %.0f | EC: %.0f\n", d.temperature,
                d.ph, d.tds, d.ec);
  Serial.printf("Rain: %d | Status: %s\n", d.raindrop, d.acidStatus.c_str());
}

void TaskReadSensors(void *pvParameters) {
  initTempSensor();
  initTDSSensor();
  initPHSensor();
  initECSensor();
  pinMode(PIN_SENSOR_HUJAN, INPUT);

  for (;;) {
    SensorData data;
    data.timestamp = get_date_time(0);        //
    data.temperature = readTempSensor();      //
    data.ph = readPHSensor(data.temperature); //
    data.tds = readTDSSensor(data.temperature);
    data.ec = readECSensor(data.temperature);
    data.raindrop = analogRead(PIN_SENSOR_HUJAN); // Baca sensor hujan
    bool isRaining = (data.raindrop < AMBANG_BATAS_HUJAN);
    data.acidStatus = calculateFuzzyStatus(data.ph, isRaining); //

    tampilkanLog(data);
    xQueueOverwrite(sensorQueue, &data); //

    vTaskDelay(SENSOR_READ_INTERVAL_MS / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(1024);
  setupWifi();
  setupTime();                                       //
  sensorQueue = xQueueCreate(1, sizeof(SensorData)); //
  xTaskCreatePinnedToCore(TaskReadSensors, "SensorTask", 8192, NULL, 2, NULL,
                          1);                                               //
  xTaskCreatePinnedToCore(taskKirimAPI, "ApiTask", 8192, NULL, 1, NULL, 0); //
}

void loop() { vTaskDelete(NULL); } //