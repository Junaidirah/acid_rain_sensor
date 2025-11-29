#ifndef SENSOR_EC_H
#define SENSOR_EC_H

#include "config.h"
#include "DFRobot_EC.h"

DFRobot_EC ec;

void initECSensor() {
    ec.begin();
}

float readECSensor(float temperature) {
    float voltage = analogRead(PIN_EC) / ADC_RESOLUTION * VOLTAGE_REF;
    float ecValue = ec.readEC(voltage, temperature);
    return ecValue;
}
#endif