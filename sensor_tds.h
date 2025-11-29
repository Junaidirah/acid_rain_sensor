#ifndef SENSOR_TDS_H
#define SENSOR_TDS_H

#include "config.h"
#include "GravityTDS.h" 

GravityTDS gravityTds;

void initTDSSensor() {
    gravityTds.setPin(PIN_TDS);
    gravityTds.setAref(3.3); 
    gravityTds.setAdcRange(4096); 
    gravityTds.begin(); 
}

float readTDSSensor(float temperature) {
    gravityTds.setTemperature(temperature); 
    gravityTds.update(); 
    float tdsValue = gravityTds.getTdsValue(); 
    return tdsValue;
}
#endif