#ifndef SENSOR_TEMP_H
#define SENSOR_TEMP_H

#include "config.h"
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(PIN_SUHU);
DallasTemperature sensors(&oneWire);

void initTempSensor() {
    sensors.begin();
}

float readTempSensor() {
    sensors.requestTemperatures(); 
    float tempC = sensors.getTempCByIndex(0);
    if(tempC == -127.00) return 25.0; 
    return tempC;
}
#endif