#ifndef AGV_SOC_H
#define AGV_SOC_H
#include "DFRobot_INA219.h"
// #include "Adafruit_INA219.h"
#include "Arduino.h"
extern double voltage, current;
void Calculate_V_and_A(DFRobot_INA219_IIC ina219);
void Calculate_SoC(float deltaT);
void saveSoCToEEPROM(float SoC);
float readSoCFromEEPROM();
#endif