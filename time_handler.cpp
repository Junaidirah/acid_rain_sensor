#include <dummy.h>
#include "time_handler.h"
#include <WiFiUdp.h>


WiFiUDP ntpUDP;
NTPClient ntp(ntpUDP, "pool.ntp.org", 25200, 3600);
ESP32Time rtc(0);


void setupTime() {
  Serial.println("Memulai NTP Client...");
  ntp.begin();
  ntp.update(); 
  
  if (ntp.update()) {
    rtc.setTime(ntp.getEpochTime());
    Serial.printf("Sukses! Waktu RTC disetel ke: %s\n", rtc.getTimeDate().c_str());
  } else {
    Serial.println("Gagal sinkronisasi NTP saat boot.");
  }
}

String get_date_time(int choose) {
  String string_month, string_day, string_hour, string_minute;
  if (rtc.getMonth() + 1 < 10) string_month = "0" + String(rtc.getMonth() + 1);
  else string_month = String(rtc.getMonth() + 1);
  if (rtc.getDay() < 10) string_day = "0" + String(rtc.getDay());
  else string_day = String(rtc.getDay());

  if (choose == 0) return String(rtc.getYear()) + "-" + string_month + "-" + string_day + " " + String(rtc.getTime());
  else if (choose == 1) return String(rtc.getYear()) + "-" + string_month + "-" + string_day;
  else if (choose == 2) return String(rtc.getTime());
  
  return "Time Error"; 
}

String get_precission_minute_second(int minute, int seconds) {
  String string_hour, string_minute, string_second;
  if (rtc.getHour(true) < 10) string_hour = "0" + String(rtc.getHour(true));
  else string_hour = String(rtc.getHour(true));
  if (minute < 10) string_minute = "0" + String(minute);
  else string_minute = String(minute);
  if (seconds < 10) string_second = "0" + String(seconds);
  else string_second = String(seconds);

  return get_date_time(1) + " " + string_hour + ":" + string_minute + ":" + string_second;
}

String get_precission_second(int seconds) {
  String string_hour, string_minute, string_second;
  if (rtc.getHour(true) < 10) string_hour = "0" + String(rtc.getHour(true));
  else string_hour = String(rtc.getHour(true));
  if (rtc.getMinute() < 10) string_minute = "0" + String(rtc.getMinute());
  else string_minute = String(rtc.getMinute());
  if (seconds < 10) string_second = "0" + String(seconds);
  else string_second = String(seconds);

  return get_date_time(1) + " " + string_hour + ":" + string_minute + ":" + string_second;
}