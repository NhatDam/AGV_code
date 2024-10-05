#include <Speed_read.h>
// Pins that attached to motor drivers to read speed
const int speedPinleft = 21; 
const int speedPinright = 20; 

// Define parameters to calculate time elapsed 
unsigned long t, tprev = 0;
float deltaT = 0;
long lastTime = 0;

// Define parameters to calculate the speed
volatile unsigned long countL = 0, countL_i =0, countR = 0, countR_i =0;
float speed_actual_left = 0, speed_actual_right= 0, raw_speed_left= 0, raw_speed_right= 0;
volatile unsigned long countL_prev=0, countR_prev=0;

// Define function to count pulses from left motor driver
void countLeftPulses() {
  int L = digitalRead(speedPinleft);
    if (L>0)
    {
      countL_i++;
    }
    else
    {
      countL_i--;
    }
}

// Define function to count pulses from right motor driver
void countRightPulses() {
  int R = digitalRead(speedPinright);
    if (R>0)
    {
      countR_i++;
    }
    else
    {
      countR_i--;
    }
}


// Calculate RPM every second
void print_RPM(){

  // Using atomic_block to safely access the variables by stop interrupt and re-activate it
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    countL = countL_i;
    countR = countR_i;
    }
  
  // Calculate the speed in Counts/second
  raw_speed_left = (countL-countL_prev)/deltaT;
  countL_prev=countL;
  raw_speed_right = (countR-countR_prev)/deltaT;
  countR_prev=countR;

  //Convert the speed to Revolutions/minute and print it out
  speed_actual_left = raw_speed_left/(pulsesPerRevolution*gearRatio)*60; //RPM
  Serial.print("Left Motor RPM: ");
  Serial.println(speed_actual_left);
  speed_actual_right = raw_speed_right/(pulsesPerRevolution*gearRatio)*60; //RPM
  Serial.print("Right Motor RPM: ");
  Serial.println(speed_actual_right);

}