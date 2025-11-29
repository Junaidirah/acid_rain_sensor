#ifndef SENSOR_PH_H
#define SENSOR_PH_H

#include "config.h"
#include "DFRobot_PH.h"

DFRobot_PH ph;

void initPHSensor() {
    ph.begin();
}

float readPHSensor(float temperature) {
    float voltage = analogRead(PIN_PH) / ADC_RESOLUTION * VOLTAGE_REF;
    float phValue = ph.readPH(voltage, temperature);
    return phValue;
}
#endif