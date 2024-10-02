#include "Arduino.h"
#include "util/atomic.h"

//Declare variables for speed reading
extern const int speedPinleft ; 
extern const int speedPinright ; 
extern long lastTime ;
extern volatile unsigned long pulseCountleft ;
extern volatile unsigned long pulseCountright ;
extern volatile unsigned long last_pulse_left ;
extern volatile unsigned long last_pulse_right;
extern float pulseDurationLeft;
extern volatile unsigned long pulseDurationRight;
extern volatile float velocity_i ;
extern float velocity;
extern volatile unsigned long countL, countL_i, countR, countR_i; 
extern volatile unsigned long countL_prev, countR_prev;
extern unsigned long pulseA, pulseB;
extern float deltaT;
extern unsigned long t, tprev;
extern float speed_actual_left, speed_actual_right, raw_speed_left, raw_speed_right;


// the motor has 5 pole pairs and 3 Hall sensor phases
const int motorPolePairs = 5;
const int hallSensors = 3;
const int pulsesPerRevolution = motorPolePairs * hallSensors;
const float gearRatio = 30.0; // Gear ratio of the 5GU 30L gearhead

void countLeftPulses();
void countRightPulses();
void print_RPM();





