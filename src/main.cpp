#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"
#include "SoC.h"
#include "SPI.h"
#include <Adafruit_GFX.h>    
#include <Adafruit_ST7789.h>
// #include <Wire.h>
// #include "DFRobot_INA219.h"

DFRobot_INA219_IIC ina219(&Wire,INA219_I2C_ADDRESS4);
#define TFT_CS  53
#define TFT_RST 14// Or set to -1 and connect to Arduino RESET pin
#define TFT_DC  15
#define TFT_MOSI 51  // Data out
#define TFT_SCLK 52  // Clock out
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1920;
float extMeterReading_mA = 1900;

void setup() {
  Serial.begin(115200);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }
  tft.init(240, 320,SPI_MODE2); 
  
  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);

  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses,RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  ina219.begin();
  // while(!Serial);
  // while(ina219.begin() != true) {
  //       Serial.println("INA219 begin faild");
  //       delay(2000);
  //   }
    ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
    Serial.println();
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("V: ");

    tft.setCursor(0, 120);
    tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
    tft.setTextSize(4);
    tft.print("I: ");
}

char _buffer[11];
void loop() {
  
  // Get current time in uS
  t = micros();  
  // Calculating elapsed time deltaT in S
  deltaT = ((float)(t - tprev))/1.0e6;
  tprev=t;
  // Serial.print("BusVoltage:   ");
  //   // Serial.print(ina219.getBusVoltage_V(), 2);
  //   Serial.print(ina219.getPower_mW()/ina219.getCurrent_mA(), 2);
  //   Serial.println("V");
  //   Serial.print("ShuntVoltage: ");
  //   Serial.print(ina219.getShuntVoltage_mV(), 3);
  //   Serial.println("mV");
  //   Serial.print("Current:      ");
  //   Serial.print(ina219.getCurrent_mA()/1000, 2);
  //   Serial.println("A");
  //   Serial.print("Power:        ");
  //   Serial.print(ina219.getPower_mW()/1000, 2);
  //   Serial.println("W");
  //   Serial.println("");
  //   delay(1000);
  // Print the output RPM from two motors

  Calculate_V_and_A(ina219);
  

  sprintf(_buffer,"%02u.%02u",(int)(voltage),(int)(voltage*100)%100);
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 0);
tft.print(_buffer);
  
  sprintf(_buffer,"%02u.%02u",(int)(current),(int)(current*100)%100);
tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
tft.setCursor(18*3, 120);
tft.print(_buffer);
  print_RPM();
  // Check the input commands
  
  switch (6) {
    case 7: //111
      stopp();
      break;
    case 6://110
      straight(0.125);
      break;
    case 5://101
      back(0.1);
      break;
    case 4://100
      left(0.034);
      break;
    case 3://011
      right(0.089);
      break;
    case 9:
      follow_line();
      break;
    default:
      stopp();
      break;
  }
}
