#ifndef SENSOR_TEMP_H
#define SENSOR_TEMP_H

#include "config.h"
#include <DallasTemperature.h>
#include <OneWire.h>


OneWire oneWire(PIN_SUHU);
DallasTemperature sensors(&oneWire);

void initTempSensor() { sensors.begin(); }

float readTempSensor() {
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);
  if (tempC == -127.00) {
    Serial.println("[ERROR] DS18B20 Error: -127.00 (Check Wiring/Pull-up)");
    return 25.0;
  }
  return tempC;
}
#endif