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

DFRobot_INA219_IIC ina219(&Wire,INA219_I2C_ADDRESS4);
// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1920;
float extMeterReading_mA = 1900;

#endif