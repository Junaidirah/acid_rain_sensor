#ifndef RAINDROP_SENSOR_H
#define RAINDROP_SENSOR_H

#include <Arduino.h>

extern int globalNilaiSensor;
extern bool globalStateHujan;

void setupRaindrop();
void taskReadRaindrop(void *parameter);

#endif