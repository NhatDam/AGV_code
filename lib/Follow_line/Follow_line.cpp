#include <Follow_line.h>

// Define following line function
void follow_line() {
  switch (count_on()){
  // The AGV stops when detecting no magnetic line
  case 0:
    stopp ();
    break;
  // Or else it keeps following line using PID
  default:
    PID ();
  }
}

// PID algorithm
void PID() {
  // Define PID parameters
  float integral = 0;
  float Kp = 0.15; //old value 0.08
  float Kd = 0.01; //old value 0.0001
  float Ki = 0.01;
  // Extract the error from the position difference of magnetic line
  error = sensor_position();
  integral = integral + error;
  // Conduct PID algorithm 
  float powerDifference = Kp * error + Kd * (error - lastError) + Ki*integral;
  lastError = error;
  // powerDifference = constrain(powerDifference, -maxSpeed, maxSpeed);
  // if(control_sig>maxSpeed)
  // {
  //   control_sig = maxSpeed;
  // }
  // else if(control_sig<-maxSpeed)
  // {
  //   control_sig = -maxSpeed;
  // }
  
  
  //Serial.print("   ");
  // Serial.println("PID");
  if (powerDifference < 0) {
    set_motor(0, maxSpeed - powerDifference, 1, maxSpeed);
  } else {
    set_motor(0, maxSpeed, 1, maxSpeed + powerDifference);
}
}

// Count the sensor bits that is ON 
int count_on() {
  count1 = 0;
  // Sensor has 16 bits of output - LOW is ON - HIGH is OFF
  for (int i = 0; i < 16; i++) {
    if (!digitalRead(input_pin[i])) { 
      count1++;
    }
  }
  return count1;
}

// Calculate and return the error between actual and desired position of magnetic line
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
    denom += input_array[i]; //  are assigned as per sensorWeight[]
    // Serial.print(numer);
    // Serial.print("   ");
    // Serial.println(denom);
  } // sensor array  
  position_value = 700 - (numer / denom); // Leftmost is -800 and rightmost is 700
  return position_value;
}