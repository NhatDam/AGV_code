#ifndef AGV_SPEEDREAD_H
#define AGV_SPEEDREAD_H

#include "Arduino.h"
#include "util/atomic.h"

//Declare variables for speed reading
#define speedPinright 2
#define speedPinleft 3

extern volatile long countL, countL_i, countR, countR_i; 

// extern float speed_actual_left, speed_actual_right, raw_speed_left, raw_speed_right;


//The motor has 5 pole pairs and 3 Hall sensor phases
// const int motorPolePairs = 5 ; // Double check P = 4
// const int hallSensors = 3;
// const int pulsesPerRevolution = motorPolePairs * hallSensors;
// const float gearRatio = 30.0; // Gear ratio of the 5GU 30L gearhead

//Declare functions
void countLeftPulses();
void countRightPulses();
long read_encoder(int wheel);
void reset_encoder(int wheel);

#endif




