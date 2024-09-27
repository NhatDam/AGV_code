#include "Arduino.h"

//Declare variables for speed reading
extern const int speedPinleft ; 
extern const int speedPinright ; 
extern long lastTime ;
extern volatile unsigned long pulseCountleft ;
extern volatile unsigned long pulseCountright ;
extern volatile unsigned long last_pulse_left ;
extern volatile unsigned long last_pulse_right;
extern volatile unsigned long pulseDurationLeft;
extern volatile unsigned long pulseDurationRight;


// the motor has 5 pole pairs and 3 Hall sensor phases
const int motorPolePairs = 5;
const int hallSensors = 3;
const int pulsesPerRevolution = motorPolePairs * hallSensors;
const float gearRatio = 30.0; // Gear ratio of the 5GU 30L gearhead

void countLeftPulses();
void countRightPulses();
void print_RPM(unsigned long currentTime);





