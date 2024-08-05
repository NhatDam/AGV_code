#include "Arduino.h"

int FR1 = 44;
int FR2 = 45;
int SV1 = 10;
int SV2 = 11;

int state;
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
}; // all the input pin connect to the sensor
int input_array[16];
byte input_value = 0b0000000000000000; // store the input value                                                            
int count1 = 0;
const int sensorweight[] = {
  0,
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
  1500
};
int position_value = 0;
int error, lastError;
int maxSpeed = 50;
int rotateSpeed = 25;


#define LEFT 69
#define RIGHT 420