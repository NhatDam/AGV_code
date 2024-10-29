#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"
#include "SoC.h"
#include "commands.h"
unsigned long t, tprev = 0;
float deltaT = 0;
long lastTime = 0;
void setup() {
  Serial.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);

  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses,RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
}

void loop() {
  // Get current time in uS
  t = micros();  
  // Calculating elapsed time deltaT in S
  deltaT = ((float)(t - tprev))/1.0e6;
  tprev=t;

 

  // Check the input commands
  
  switch (6) {
    case 7: //111
      stopp();
      break;
    case 6://110
      straight(0.125,0.125);
      break;
    case 5://101
      back(0.125,0.125);
      break;
    case 4://100
      left(0.125,0.125);
      break;
    case 3://011
      right(0.125,0.125);
      break;
    case 9:
      follow_line();
      break;
    default:
      stopp();
      break;
  }
}
