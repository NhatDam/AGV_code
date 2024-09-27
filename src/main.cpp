#include <Follow_line.h>
#include "Speed_read.h"

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
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  static unsigned long lastTime = 0;
  unsigned long currentTime = millis();
  
   // Convert the analog value to voltage (assuming a 5V reference)
  float voltage_left = speed_left * (5.0 / 255);
  float voltage_right = speed_left * (5.0 / 255);
  check();
  sensor_position();
  // Motor control logic
  Serial.println(state);
  switch (state) {
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
    case 9:
      follow_line();
      break;
    default:
      stopp();
      break;
  }
}
