#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"
#include "SoC.h"
#include "SPI.h"
#include "Oled.h"
#include "SimpleKalmanFilter.h"
// #define TFT_CS  53
// #define TFT_RST 14// Or set to -1 and connect to Arduino RESET pin
// #define TFT_DC  15
// #define TFT_MOSI 51  // Data out
// #define TFT_SCLK 52  // Clock out
DFRobot_INA219_IIC ina219(&Wire,INA219_I2C_ADDRESS4);
// Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
SimpleKalmanFilter speed_read_left(1, 1, 0.01);
SimpleKalmanFilter speed_read_right(1, 1, 0.01);

// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1920;
float extMeterReading_mA = 1900;
float F_speedL = 0, F_speedR = 0;
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }
   
  
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

  // 2.0 TFT oled initialize
  // OLED_init(tft);
}


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
  print_RPM();
  F_speedL = speed_read_left.updateEstimate(speed_actual_left);
  if(F_speedL<1)
  {
    F_speedL = 0;
  }
  F_speedR = speed_read_right.updateEstimate(speed_actual_right);
  if(F_speedR<1)
  {
    F_speedR = 0;
  }
  Calculate_V_and_A(ina219);
  Calculate_SoC(deltaT, current);
  
  
  // Serial.print("Filter Left: "); Serial.println(F_speedL,3);
  // Serial.print("Filter Right: "); Serial.println(F_speedR,3);
  // Serial.print("Left: "); Serial.println(speed_actual_left,3); 
  // Serial.print("Right: "); Serial.println(speed_actual_right,3);
  // Serial.print(SoC_percentage,3); Serial.println();

  Serial2.print(voltage); Serial2.print(",");
  Serial.print(voltage); Serial.print(",");
  Serial2.print(current); Serial2.print(",");
  Serial.print(current); Serial.print(",");
  Serial2.print(F_speedL); Serial2.print(",");
  Serial.print(F_speedL); Serial.print(",");
  Serial2.print(F_speedR); Serial2.print(",");
  Serial.print(F_speedR); Serial.print(",");
  Serial2.print(SoC_percentage); Serial2.println();
  Serial.print(SoC_percentage); Serial.println();
  
  
  // Serial.print(SoC_percentage,3); Serial.println();
  // Serial.print(deltaT);
  // Check the input commands
  
  switch (9) {
    case 7: //111
      stopp();
      break;
    case 6://110
      straight(50);
      break;
    case 5://101
      back(0.125);
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
