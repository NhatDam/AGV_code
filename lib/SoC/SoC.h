#ifndef AGV_SOC_H
#define AGV_SOC_H
#include "DFRobot_INA219.h"
// #include "Adafruit_INA219.h"
#include "Arduino.h"
extern float voltage, current;
extern float SoC_percentage, pw_consump;
void Calculate_V_and_A(DFRobot_INA219_IIC ina219);
void Calculate_SoC(float deltaT, float current);
void saveSoCToEEPROM(float SoC);
float readSoCFromEEPROM();
#endif