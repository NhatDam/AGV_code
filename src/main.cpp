#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"

float voltageToRPM(float voltage) {
  // 5V corresponds to 3000 RPM:
  float conversionFactor = 3000.0 / 5.0;

  // Convert the voltage to RPM
  float rpm = voltage * conversionFactor;

  return rpm;
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
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(A1, INPUT);
}

void loop() {
  unsigned long curTime = micros();
  if(curTime>=2e6)
  {
    check();
    sensor_position();
    print_RPM(curTime);
    // Motor control logic
    // Serial.println(state);
    switch (state) {
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
        follow_line(curTime);
        break;
      default:
        stopp();
        break;
    }
}
}
