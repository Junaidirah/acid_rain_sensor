#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// --- KONFIGURASI PIN ---
#define PIN_PH 32
#define PIN_TDS 33
#define PIN_EC 34
#define PIN_SUHU 4
#define PIN_SENSOR_HUJAN 35 // Gunakan Pin 35 untuk Sensor Hujan

// --- KONFIGURASI SYSTEM ---
#define ADC_RESOLUTION 4096.0
#define SENSOR_READ_INTERVAL_MS 15000
#define AMBANG_BATAS_HUJAN 3000
#define VOLTAGE_REF 3300.0
#define API_SEND_INTERVAL_MS 120000

// --- WIFI & SERVER ---
extern const char *ssid;
extern const char *password;
extern const char *apiEndpoint;

// --- STRUKTUR DATA ---
struct SensorData {
  String timestamp;
  float ph;
  float tds;
  float ec;
  float temperature;
  int raindrop; // Nilai mentah ADC
  String acidStatus;
};

#endif