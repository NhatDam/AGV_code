#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"
#include "SoC.h"
#include "SPI.h"
#include "Oled.h"
#include "SimpleKalmanFilter.h"
DFRobot_INA219_IIC ina219(&Wire,INA219_I2C_ADDRESS4);

// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1920;
float extMeterReading_mA = 1900;


void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }
   
  
  // pinMode(speedPinleft, INPUT);
  // pinMode(speedPinright, INPUT);

  // attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses,RISING);
  // attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);
  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  ina219.begin();
  ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);

}


void loop() {
  


 // main1.cpp
  Calculate_V_and_A(ina219);
  Calculate_SoC(voltage);
  Power_consumption(current, voltage);

    // -------- USB console --------
      Serial.print("Voltage: ");
  Serial.println(voltage); 
     Serial.print("Current: ");
  Serial.println(current);
    Serial.print(F("[BAT]: "));
    Serial.print(SoC_percentage, 1);  Serial.println(F("%,  "));
    delay(500);
    Serial.print(F("[POW CONSUMPT] "));
    Serial.print(pw_consump, 2);      Serial.println(F("W,  "));

}



  


