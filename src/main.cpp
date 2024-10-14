#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"


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
  // pinMode(3, INPUT_PULLUP);
  // pinMode(6, INPUT_PULLUP);
  // pinMode(7, INPUT_PULLUP);
  pinMode(A1, INPUT);
}

void loop() {
  t = micros();  // Get current time in uS
  // Calculating elapsed time deltaT
  deltaT = ((float)(t - tprev))/1.0e6;
  tprev=t;

  // Print the output RPM from two motors
  print_RPM();
  // Check the input commands
  check();

  
  switch (6) {
    case 7: //111
      stopp();
      break;
    case 6://110
      straight();
      break;
    case 5://101
      back();
      break;
    case 4://100
      left();
      break;
    case 3://011
      right();
      break;
    case 9:
      follow_line();
      break;
    default:
      stopp();
      break;
  }
}
