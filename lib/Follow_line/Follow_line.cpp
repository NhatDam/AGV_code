#include <Follow_line.h>

void follow_line() {
  switch (count_on())
  {
  case 0:
    stopp ();
    break;
  default:
    PID ();
  }
  
  
}

void PID() {
  float Kp = 0.15; //old value 0.08
  float Kd = 0.0005; //old value 0.0001
  error = sensor_position();
  float powerDifference = Kp * error + Kd * (error - lastError);
  lastError = error;
  powerDifference = constrain(powerDifference, -maxSpeed, maxSpeed);
  //Serial.print(powerDifference);
  //Serial.print("   ");
  // Serial.println("PID");
  if (powerDifference < 0) {
    set_motor(0, maxSpeed - powerDifference, 1, maxSpeed);
  } else {
    set_motor(0, maxSpeed, 1, maxSpeed + powerDifference);
  }
}

int count_on() {
  count1 = 0;
  for (int i = 0; i < 16; i++) {
    if (!digitalRead(input_pin[i])) { // count number of sensor on the line
      count1++;
    }
  }
  return count1;
}

int sensor_position() {
  int input_array[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  };
  for (int i = 0; i < 16; i++) {
    input_array[i] = 1 - digitalRead(input_pin[i]);
  }
  position_value = 0;
  int numer = 0;
  int denom = 0;
  for (int i = 0; i < 16; i++) {
    numer += input_array[i] * sensorweight[i]; //  Calculates the position of the sensor array over the line by weighted average method. The weights
    denom += input_array[i]; //  are assigned as per sensorWeight[]. A value of -7500 indicates that the line is over the leftmost
    // Serial.print(numer);
    // Serial.print("   ");
    // Serial.println(denom);
  } // sensor array  
  position_value = 700 - (numer / denom);
  return position_value;
}