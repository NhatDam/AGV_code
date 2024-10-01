#include "GPIO.h"
int FR1 = 44;
int FR2 = 45;
int SV1 = 11;
int SV2 = 10;
int position_value = 0;
float error, lastError;
int maxSpeed = 70; //min = 15 && max = 70
int rotateSpeed = 25;
int count1 = 0;
byte input_value = 0b0000000000000000;
int input_pin[] = {
  A15,
  A14,
  A13,
  A12,
  A11,
  A10,
  A9,
  A8,
  A7,
  A6,
  A5,
  A4,
  A3,
  4,
  5,
  2
};
const int sensorweight[] = {
  0,//leftmost
  100,
  200,
  300,
  400,
  500,
  600,
  700,
  800,
  900,
  1000,
  1100,
  1200,
  1300,
  1400,
  1500 //rightmost
};