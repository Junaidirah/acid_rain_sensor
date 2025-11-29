#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <EEPROM.h> 

// --- KONFIGURASI PIN ---
#define PIN_PH    32
#define PIN_TDS   33
#define PIN_EC    34
#define PIN_SUHU  4

// --- KONFIGURASI SYSTEM ---
#define ADC_RESOLUTION 4096.0
#define VOLTAGE_REF    3300.0 
#define API_SEND_INTERVAL_MS 120000 // 2 Menit (120.000 ms)

// --- WIFI & SERVER ---
const char* ssid = "NAMA_WIFI_ANDA";
const char* password = "PASSWORD_WIFI_ANDA";
const char* apiEndpoint = "https://api.example.com/data"; 

// --- STRUKTUR DATA ---
struct SensorData {
  String timestamp;
  float ph;
  float tds;
  float ec;
  float temperature;
  String acidStatus;
};

#endif