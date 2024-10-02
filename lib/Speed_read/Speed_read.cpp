#include <Speed_read.h>
const int speedPinleft = 21; 
const int speedPinright = 20; 
long lastTime = 0;
volatile unsigned long pulseCountleft = 0;
volatile unsigned long pulseCountright = 0;
volatile unsigned long last_pulse_left = 0;
volatile unsigned long last_pulse_right = 0;
float pulseDurationLeft = 0;
volatile unsigned long pulseDurationRight = 0;
volatile float velocity_i = 0;
float velocity = 0;
volatile unsigned long countL = 0, countL_i =0, countR = 0, countR_i =0;
unsigned long t, tprev = 0, tprev1 = 0;
float deltaT = 0;
float speed_actual_left = 0, speed_actual_right= 0, raw_speed_left= 0, raw_speed_right= 0;
volatile unsigned long countL_prev=0, countR_prev=0;

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
ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
  countL = countL_i;
  countR = countR_i;
  }
  raw_speed_left = (countL-countL_prev)/deltaT;
  countL_prev=countL;
  raw_speed_right = (countR-countR_prev)/deltaT;
  countR_prev=countR;

  //Convert raw to real speed
  speed_actual_left = raw_speed_left/(pulsesPerRevolution*gearRatio)*60; //RPM
  Serial.print("Left Motor RPM: ");
  Serial.println(speed_actual_left);
  speed_actual_right = raw_speed_right/(pulsesPerRevolution*gearRatio)*60; //RPM
  Serial.print("Right Motor RPM: ");
  Serial.println(speed_actual_right);

}