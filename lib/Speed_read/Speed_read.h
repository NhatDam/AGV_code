#include "Arduino.h"

//Declare variables for speed reading
const int speedPinleft = 21; 
const int speedPinright = 20; 
unsigned long lastTime = 0;
volatile unsigned long pulseCountleft = 0;
volatile unsigned long pulseCountright = 0;
volatile unsigned long last_pulse_left = 0;
volatile unsigned long last_pulse_right = 0;
volatile unsigned long pulseDurationLeft = 0;
volatile unsigned long pulseDurationRight = 0;

// the motor has 5 pole pairs and 3 Hall sensor phases
const int motorPolePairs = 5;
const int hallSensors = 3;
const int pulsesPerRevolution = motorPolePairs * hallSensors;
const float gearRatio = 30.0; // Gear ratio of the 5GU 30L gearhead

void countLeftPulses();
void countRightPulses();
void print_RPM(unsigned long currentTime);





