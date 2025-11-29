#ifndef TIME_HANDLER_H
#define TIME_HANDLER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ESP32Time.h>

// Expose objek agar bisa diakses file lain
extern NTPClient ntp;
extern ESP32Time rtc;

// Deklarasi Fungsi
void setupTime();
String get_date_time(int choose);

#endif