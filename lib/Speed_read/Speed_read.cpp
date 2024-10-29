#include <Speed_read.h>


// Define parameters to calculate the speed
volatile long countL = 0, countL_i =0, countR = 0, countR_i =0;

// Define function to count pulses from left motor driver
void countLeftPulses() {
      countL_i++;
}

// Define function to count pulses from right motor driver
void countRightPulses() {
      countR_i++;
}

// Calculate RPM every second
long read_encoder(int wheel){

  // Using atomic_block to safely access the variables by stop interrupt and re-activate it
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
    countL = countL_i;
    countR = countR_i;
    }
  if(wheel== 0)
  {
    return countL;
  }
  else if(wheel == 1)
  {
    return countR;
  }
  
}