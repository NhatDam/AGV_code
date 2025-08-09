#include <SoC.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SimpleKalmanFilter.h>


// int eepromAddress = 0;
// // config 
// float battery_cap = 35;//35Ah
// unsigned long save_interval = 10000;

// live state (file-scope, no re-declare in funcytions)
float voltage, current = 0;
float SoC_percentage = 0; 
// float pw_consump = 0;



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


// --- how many 12 V blocks in series (2x 12V = 24V pack) ---
// static const int S = 2;

// ---------- 12 V lead-acid OCV → SoC (per battery; then scaled to pack) ----------
//  float estimateSoCFromVoltage(float v_pack)
// {
//     // Convert 24 V pack voltage to equivalent 12 V battery voltage
//     float v12 = v_pack / S;

//     // Piecewise-linear lookup (rested battery at ~25°C; adjust if you have a datasheet)
//     const struct { float V; float s; } T[] = {
//         {11.80f, 0.00f},  // ~0 %
//         {12.00f, 0.25f},  // ~25 %
//         {12.20f, 0.50f},  // ~50 %
//         {12.40f, 0.75f},  // ~75 %
//         {12.60f, 0.95f},  // ~95 %
//         {12.70f, 1.00f},  // ~100 %
//     };

//     if (v12 <= T[0].V) return 0.0f;
//     if (v12 >= T[5].V) return 1.0f;
//     for (int i = 0; i < 5; ++i) {
//         if (v12 < T[i+1].V) {
//             float t = (v12 - T[i].V) / (T[i+1].V - T[i].V);
//             return T[i].s + t * (T[i+1].s - T[i].s);
//         }
//     }
//     return 1.0f;
// }


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


