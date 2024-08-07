#include "Arduino.h"
void print_RPM(unsigned long currentTime);
//Declare variables for speed reading
const int speedPinleft = 18; 
const int speedPinright = 19; 
unsigned long lastTime = 0;
unsigned int pulsesPerRevolution = 5;


// void countLeftPulses() {
//   unsigned long currentTime = millis ();
//   pulseCountleft = currentTime - last_pulse_left;
//   last_pulse_left = currentTime;
// };

// void countRightPulses() {
//   unsigned long currentTime = millis ();
//   pulseCountright = currentTime - last_pulse_right;
//   last_pulse_right = currentTime;
// };


  

// Calculate RPM every second
void print_RPM(unsigned long currentTime){
  if (currentTime - lastTime >= 1000000) {
    noInterrupts(); // Disable interrupts temporarily to read pulseCount
    // // float countleft = 1000000.0/ pulseCountleft;
    // // float countright = 1000000.0/ pulseCountright;
    //  // Calculate RPM
    // int LeftRPM = ((countleft / pulsesPerRevolution) * 60.0);
    // int RightRPM = ((countright / pulsesPerRevolution) * 60.0);
    // // Print RPM values
    // Serial.print("Left Motor RPM: ");
    // Serial.println(countleft);
    // Serial.print("Right Motor RPM: ");
    // Serial.println(0);
    // pulseCountleft = 0; // Reset pulse count
    // pulseCountright = 0;
    // interrupts(); // Re-enable interrupt
   
    // Update the lastTime
    lastTime = currentTime;
  }
}



