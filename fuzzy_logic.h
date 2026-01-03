#ifndef FUZZY_LOGIC_H
#define FUZZY_LOGIC_H

#include <Arduino.h>

String calculateFuzzyStatus(float phVal, bool isRaining) {
  if (!isRaining) {
    return "KERING";
  }

  String status = "";
  float isAcidRain = 0.0;
  float isNormalRain = 0.0;

  // 1. Fuzzification
  if (phVal <= 5.6)
    isAcidRain = 1.0;
  else if (phVal > 5.6 && phVal < 6.0)
    isAcidRain = (6.0 - phVal) / (6.0 - 5.6);

  if (phVal > 5.6 && phVal <= 7.0)
    isNormalRain = 1.0;
  else if (phVal > 5.0 && phVal <= 5.6)
    isNormalRain = (phVal - 5.0) / (5.6 - 5.0);

  // 2. Defuzzification (Logic)
  if (phVal < 5.0)
    status = "HUJAN ASAM (Kritis)";
  else if (isAcidRain >= 0.8)
    status = "BAHAYA: Hujan Asam";
  else if (isAcidRain > 0.0)
    status = "WASPADA: Cenderung Asam";
  else if (phVal > 7.5)
    status = "BASA";
  else
    status = "NORMAL";

  return status;
}
#endif