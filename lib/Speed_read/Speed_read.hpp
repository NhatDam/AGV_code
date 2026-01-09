#ifndef AGV_SPEEDREAD_H
#define AGV_SPEEDREAD_H
#include "../../include/commands.h"
#include "Arduino.h"
#include "util/atomic.h"
#include "SimpleKalmanFilter.h"
//Declare variables for speed reading
#define speedPinright 2
#define speedPinleft 3

extern volatile long  countL, countL_i, countR, countR_i; 
extern float speed_left, speed_right;
extern unsigned char reverse_L;
extern unsigned char reverse_R;

//The motor has 5 pole pairs and 3 Hall sensor phases
// const int motorPolePairs = 5 ; // Double check P = 4
// const int hallSensors = 3;
// const int pulsesPerRevolution = motorPolePairs * hallSensors;
// const float gearRatio = 30.0; // Gear ratio of the 5GU 30L gearhead
#define CPR 450  // 3 hall sensors × 5 pole pairs × 30 gear ratio
//Declare functions
void countLeftPulses();
void countRightPulses();
long read_encoder(int wheel);
void reset_encoder(int wheel);
void local_RPM(float deltaT);
float get_speed_rad_per_sec(int wheel);
#endif




