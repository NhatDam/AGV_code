#ifndef AGV_SOC_H
#define AGV_SOC_H
#include "DFRobot_INA219.h"
#include "Arduino.h"     
extern float voltage, current;
extern float SoC_percentage;
extern float pw_consump;
extern float time_remaining; // in hours
void Time_remaining(float voltage, float current, float battery_cap);
void Calculate_V_and_A(DFRobot_INA219_IIC ina219);
void Calculate_SoC(float voltage);
void Power_consumption(float current, float voltage);
#endif