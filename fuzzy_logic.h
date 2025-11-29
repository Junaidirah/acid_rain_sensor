#ifndef FUZZY_LOGIC_H
#define FUZZY_LOGIC_H

#include <Arduino.h>

String calculateFuzzyStatus(float phVal) {
  String status = "";
  
  float isAcidRain = 0.0;
  float isNormalRain = 0.0;

  // 1. Fuzzification
  if (phVal <= 5.6) isAcidRain = 1.0;
  else if (phVal > 5.6 && phVal < 6.0) isAcidRain = (6.0 - phVal) / (6.0 - 5.6);
  else isAcidRain = 0.0;

  if (phVal > 5.6 && phVal <= 7.0) isNormalRain = 1.0;
  else if (phVal > 5.0 && phVal <= 5.6) isNormalRain = (phVal - 5.0) / (5.6 - 5.0);
  else isNormalRain = 0.0;

  // 2. Defuzzification
  if (isAcidRain >= 0.8) status = "BAHAYA: Hujan Asam"; 
  else if (isAcidRain > 0.0 && isAcidRain < 0.8) status = "WASPADA: Cenderung Asam";
  else if (isNormalRain >= 0.5) status = "NORMAL";
  else if (phVal > 7.0) status = "BASA";
  else status = "UNDEFINED";
  
  // // 3. Hard Rule
  // if (phVal < 5.6) status = "HUJAN ASAM (Critical)";

  return status;
}

#endif