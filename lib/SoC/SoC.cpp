#include <SoC.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SimpleKalmanFilter.h>

// live state (file-scope, no re-declare in funcytions)
float voltage, current = 0;
float pw_consump = 0; // Power consumption in Watts
float SoC_percentage = 0; 
// float pw_consump = 0;


#if 0
void Calculate_V_and_A(DFRobot_INA219_IIC ina219)
{
    voltage = ina219.getBusVoltage_V();
    if(voltage < 0.1)
    {
        voltage = 0;
    }

    current = (ina219.getCurrent_mA())/1000;
    if(current < 0.05)
    {
        current = 0;
    }
}
#else
void Calculate_V_and_A(DFRobot_INA219_IIC ina219)
{
    voltage = 25.4;
    if(voltage < 0.1)
    {
        voltage = 0;
    }

    current = 2500/1000;
    if(current < 0.05)
    {
        current = 0;
    }
}
#endif


//Calculate the current SoC of the battery
void Calculate_SoC(float voltage)
{

    float Max_Voltage = 25.4;
    float Min_Voltage = 23.0;
    if (voltage > Max_Voltage) {
        SoC_percentage = 100.0; // Fully charged
    } else if (voltage < Min_Voltage) {
        SoC_percentage = 0.0; // Fully discharged
    } else {
        // Linear interpolation between Min_Voltage and Max_Voltage
        SoC_percentage = ((voltage - Min_Voltage) / (Max_Voltage - Min_Voltage)) * 100.0;
    }


}

void Power_consumption(float current, float voltage){

pw_consump = current * voltage; // Power in Watts


}

