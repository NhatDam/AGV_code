#include <Speed_read.h>
const int speedPinleft = 21; 
const int speedPinright = 20; 
long lastTime = 0;
// volatile unsigned long pulseCountleft = 0;
// volatile unsigned long pulseCountright = 0;
volatile unsigned long last_pulse_left = 0;
volatile unsigned long last_pulse_right = 0;
volatile unsigned long pulseDurationLeft = 0;
volatile unsigned long pulseDurationRight = 0;
void countLeftPulses() {
  unsigned long currentTimeleft = micros();
  if (currentTimeleft - last_pulse_left > 1000) { // Debounce: ignore pulses shorter than 1000 microseconds
    pulseDurationLeft = currentTimeleft - last_pulse_left;
    last_pulse_left = currentTimeleft;
    // pulseCountleft++;
  }
}

void countRightPulses() {
  unsigned long currentTimeright = micros();
  if (currentTimeright - last_pulse_right > 1000) { // Debounce: ignore pulses shorter than 1000 microseconds
    pulseDurationRight = currentTimeright - last_pulse_right;
    last_pulse_right = currentTimeright;
    // pulseCountright++;
  }
}


// Calculate RPM every second
void print_RPM(unsigned long currentTime){
  
  if (currentTime - lastTime >= 1000) {
    lastTime = currentTime;

    // Calculate RPM for the left motor
    if (pulseDurationLeft > 0) {
      float frequencyLeft = 1000000.0 / pulseDurationLeft; // Convert microseconds to Hz
      float motorRPMLeft = (frequencyLeft * 60.0) / pulsesPerRevolution;
      float outputRPMLeft = motorRPMLeft / gearRatio;
      Serial.print("Left Motor RPM: ");
      Serial.println(outputRPMLeft);
    } else {
      Serial.println("Left Motor RPM: 0");
    }

    // Calculate RPM for the right motor
    if (pulseDurationRight > 0) {
      float frequencyRight = 1000000.0 / pulseDurationRight; // Convert microseconds to Hz
      float motorRPMRight = (frequencyRight * 60.0) / pulsesPerRevolution;
      float outputRPMRight = motorRPMRight / gearRatio;
      Serial.print("Right Motor RPM: ");
      Serial.println(outputRPMRight);
    } else {
      Serial.println("Right Motor RPM: 0");
    }

    // Reset pulse counts for the next interval
    // pulseCountleft = 0;
    // pulseCountright = 0;
  }
}