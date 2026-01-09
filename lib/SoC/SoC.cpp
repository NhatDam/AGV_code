#include <SoC.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SimpleKalmanFilter.h>



int eepromAddress = 0;
float battery_cap = 35;//35Ah
float voltage, current = 0;
float SoC_percentage = 0; 
unsigned long save_interval = 10000;
float pw_consump = 0;
float current_SoC = 0;
void Calculate_V_and_A(DFRobot_INA219_IIC ina219)
{
    voltage = ina219.getBusVoltage_V();
    if(voltage < 0.1)
    {
        voltage = 0;
    }
    // voltage = ina219.getPower_mW()/ina219.getCurrent_mA();
    // Serial.print("Voltage: ");
    // Serial.print(voltage);
    // Serial.println(" V");

    current = (ina219.getCurrent_mA())/1000;
    if(current < 0.05)
    {
        current = 0;
    }
    // current = 9;
    // Serial.print("Current: ");
    // Serial.print(current);
    // Serial.println(" A");
    
}
//Calculate the current SoC of the battery
void Calculate_SoC(float deltaT, float current){
    unsigned long last_save_time = 0;
    float previous_SoC = readSoCFromEEPROM();

    pw_consump = pw_consump + ((current/battery_cap)*(deltaT/3600));
    
    if(millis() - last_save_time > save_interval || millis() < 1000 )
    {
        current_SoC = previous_SoC - pw_consump;
        pw_consump = 0;
        saveSoCToEEPROM(current_SoC);
        last_save_time = millis();
    }
    
    SoC_percentage = current_SoC * 100;;
    // Serial.print("SOC: ");
    // Serial.println(SoC_percentage);

}
// Write the SoC value to EEPROM
void saveSoCToEEPROM(float SoC) {
    EEPROM.put(eepromAddress, SoC);
}
// Read the SoC value from EEPROM
float readSoCFromEEPROM() {
    float storedSoC = 0.0;
    EEPROM.get(eepromAddress, storedSoC);
    if (storedSoC < 0.0 || storedSoC > 1.0) {
        storedSoC = 1.0;  // Initialize to 100% if not set
    }
    
    return storedSoC;
}