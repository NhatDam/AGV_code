#include <SoC.h>
#include <Wire.h>
#include <EEPROM.h>
DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);
int eepromAddress = 0;
float battery_cap = 35;//35Ah
float voltage, current = 0;
unsigned long save_interval = 60000;

void Calculate_V_and_A()
{
    voltage = ina219.getBusVoltage_V();
    Serial.print("Voltage: ");
    Serial.println(voltage);

    current = (ina219.getCurrent_mA())/1000;
    Serial.print("Current: ");
    Serial.println(current);
    
}
//Calculate the current SoC of the battery
void Calculate_SoC(float deltaT){
    unsigned long last_save_time = 0;
    float previous_SoC = readSoCFromEEPROM();
    float current_SoC = previous_SoC - ((current/battery_cap)*(deltaT/3600));
    if(millis() - last_save_time > save_interval)
    {
        saveSoCToEEPROM(current_SoC);
        last_save_time = millis();
    }
    float SoC_percentage = current_SoC * 100;
}
// Write the SoC value to EEPROM
void saveSoCToEEPROM(float SoC) {
    EEPROM.put(eepromAddress, SoC);
}
// Read the SoC value from EEPROM
float readSoCFromEEPROM() {
    float storedSoC = 0.0;
    EEPROM.get(eepromAddress, storedSoC);
    if (storedSoC < 0.0 || storedSoC > 1.0 || voltage > 25.0) {
        storedSoC = 1.0;  // Initialize to 100% if not set
    }
    
    return storedSoC;
}

