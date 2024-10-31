#ifndef AGV_SOC_H
#define AGV_SOC_H
#include "DFRobot_INA219.h"
#include "Arduino.h"
void Calculate_V_and_A();
void Calculate_SoC(float deltaT);
void saveSoCToEEPROM(float SoC);
float readSoCFromEEPROM();
void battery_init();
extern float voltage, current;
#endif