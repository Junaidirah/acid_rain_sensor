#ifndef SENSOR_TDS_H
#define SENSOR_TDS_H

#include "GravityTDS.h"
#include "config.h"

GravityTDS gravityTds;

void initTDSSensor() {
  gravityTds.setPin(PIN_TDS);
  gravityTds.setAref(3.3);
  gravityTds.setAdcRange(4096);
  gravityTds.setKvalueAddress(
      0x64); // Set EEPROM address to 100 to avoid conflict with EC/PH
  gravityTds.begin();
}

float readTDSSensor(float temperature) {
  gravityTds.setTemperature(temperature);
  gravityTds.update();
  float tdsValue = gravityTds.getTdsValue();
  return tdsValue;
}
#endif