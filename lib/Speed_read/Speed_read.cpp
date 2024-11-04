#include <Speed_read.h>
#include "D:/RMIT/AGV_Project_2024/AGV_code/AGV_code/include/commands.h"
#include "control.h"

float speed_left = 0, speed_right = 0;
// Define parameters to calculate the speed
volatile long  countL = 0, countL_i =0, countR = 0, countR_i =0;
volatile long  countL_prev = 0, countR_prev = 0;

// Define function to count pulses from left motor driver
void countLeftPulses() {
  if(reverse_L == 0) countL_i++;
  else if(reverse_L == 1) countL_i--;
}

// Define function to count pulses from right motor driver
void countRightPulses() {
  if(reverse_R == 0) 
  {
    countR_i++;
  }
  else if(reverse_R == 1){
    countR_i--;
  } 

}

// Calculate RPM every second
long read_encoder(int wheel){

  // Using atomic_block to safely access the variables by stop interrupt and re-activate it
  // ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
  //   countL = countL_i;
  //   countR = countR_i;
  //   }
  if(wheel == LEFT)
  {
    return countL_i;
  }
  else 
  {
    return countR_i;
  }
}

void reset_encoder(int wheel)
{
  if(wheel== LEFT)
  {
    countL_i = 0;
  }
  else 
  {
    countR_i = 0;
  }
}

void local_RPM(float deltaT)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    countL = countL_i;
    countR = countR_i;
    }
  speed_left = (countL_i-countL_prev)/deltaT;
  countL_prev = countL;
  speed_right = (countR_i-countR_prev)/deltaT;
  countR_prev = countR;

}