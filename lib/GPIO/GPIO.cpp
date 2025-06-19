#include "GPIO.h"
// '1' is RIGHT motor and '2' is LEFT motor
// Define motor direction control pins
int FR1 = 44;
int FR2 = 45;

// Define motor speed control pins
int SV1 = 11;
int SV2 = 10;
int position_value = 0;
int error, lastError;
int maxSpeed = 10;
int rotateSpeed = 25;
int count1 = 0;
byte input_value = 0b0000000000000000;
// Define the input pins for magnetic line sensor
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
  6
};

// Divide 16 bits of input from the magnetic sensor into 16 different equal-distributed values starting from 0 to 1500
const int sensorweight[] = {
  0, // leftmost the sensor
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
  1500 // rightmost of the sensor
};