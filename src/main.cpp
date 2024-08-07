#include <Follow_line.h>
#include "Speed_read.h"
volatile unsigned long pulseCountleft = 0;
volatile unsigned long pulseCountright = 0;
volatile unsigned long last_pulse_left = 0;
volatile unsigned long last_pulse_right = 0;

void countLeftPulses() {
  unsigned long currentTimeleft = micros();
  if (currentTimeleft - last_pulse_left > 1000) { // Debounce: ignore pulses shorter than 1000 microseconds
    pulseCountleft = currentTimeleft - last_pulse_left;
    Serial.print("Current Time: ");
    Serial.println(currentTimeleft);
    Serial.print("Last Pulse Time: ");
    Serial.println(last_pulse_left);
    Serial.print("Pulse Interval: ");
    Serial.println(pulseCountleft);
    last_pulse_left = currentTimeleft;
  }
}

void countRightPulses() {
  unsigned long currentTimeright = micros();
  if (currentTimeright - last_pulse_right > 1000) { // Debounce: ignore pulses shorter than 1000 microseconds
    pulseCountright = currentTimeright - last_pulse_right;
    last_pulse_right = currentTimeright;
  }
}

void setup() {
  Serial.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }
  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);
  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses, RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  unsigned long currentTime = micros();
  float countleft = 1000000.0 / pulseCountleft;
  float countright = 1000000.0 / pulseCountright;
  int LeftRPM = ((countleft / pulsesPerRevolution) * 60.0);
  int RightRPM = ((countright / pulsesPerRevolution) * 60.0);
  Serial.print("Left Motor RPM: ");
  Serial.println(LeftRPM);
  Serial.print("Right Motor RPM: ");
  Serial.println(RightRPM);
  delay(1000);
  switch (6) {
    case 7:
      stopp();
      break;
    case 6:
      straight();
      break;
    case 5:
      back();
      break;
    case 4:
      left();
      break;
    case 3:
      right();
      break;
    case 0:
      follow_line();
      break;
    default:
      stopp();
      break;
  }
}
