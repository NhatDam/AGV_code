#include "Follow_line.hpp"

PID_CLASS motorL(2, 2, 0.1, LEFT); 
PID_CLASS motorR(2, 2, 0.1, RIGHT);
boolean flag1 = 1;
int count_plant = 0;
long current_time = 0;
int prev_detected = 0; // Added to track previous state of marker detection
int marked_plant[MAX_SIZE] = {0}; // 1 = sicked, 0 = healthy
boolean light_on = 0;

// Define following line function
void follow_line(long t) {
  
  switch (count_on()){
  // The AGV stops when detecting no magnetic line
  case 0:
    // Serial.println("Halted Follow");
    stopp();
    break;
  // Or else it keeps following line using PID
  default:
   if (!light_on && t >= 3000000)
   {
    PID ();
   }
  }
}


void UVC(long time){
  int left_pin_1 = 1-digitalRead(input_pin[0]);
  int left_pin_2 = 1-digitalRead(input_pin[1]);
  int right_pin_1 = 1-digitalRead(input_pin[14]);
  int right_pin_2 = 1-digitalRead(input_pin[15]);
  // Read the magnetic line sensor
  // for (int i = 0; i < 16; i++) {
  //   input_array2[i] = 1 - digitalRead(input_pin[i]);
  // }

  // Start condition: AGV detects both left and right markers
  if ((left_pin_1 == 1 || left_pin_2 == 1) && (right_pin_1 == 1 || right_pin_2 == 1)) {
    count_plant = 0;
  }

  // End condition: AGV sees only left marker
  

  
    int current_detected = (left_pin_1 == 1 || left_pin_2 == 1) ? 1 : 0;

    if (current_detected == 1 && prev_detected == 0) {
      // Rising edge detected → count new plant section
      count_plant++;

      if (count_plant>0 && count_plant < 9) {
        if (marked_plant[count_plant-1] == 1) {
          if (light_on == 0)
          {
            // Serial.println("Light ON");
            
            // Serial.println("Halted UVC");
            agvHalt();
            digitalWrite(15, HIGH); // UVC light ON
            light_on = 1;
            current_time = time;
          }
        } else {
          
          digitalWrite(15, LOW);
          // Serial.println(count_plant);  // UVC light OFF
          
        }
      }
    }
    if(((time - current_time) >=10000000)&& light_on == 1){
            agvResume();
            marked_plant[count_plant-1] = 0;
            light_on = 0;
            digitalWrite(15, LOW);  // UVC light OFF
          }
    // Update previous state
    prev_detected = current_detected;
}

// PID algorithm
void PID() {
  // Define PID parameters
  float integral = 0;
  float Kp = 0.1; //old value 0.08
  float Kd = 2.5; //old value 0.0001
  float Ki = 0.001;
  // Extract the error from the position difference of magnetic line
  error = sensor_position();
  integral = integral + error;
  // Conduct PID algorithm 
  float powerDifference = Kp * error + Kd * (error - lastError) + Ki*integral;
  lastError = error;

  if (powerDifference < 0) {
    // set_motor(CCW, maxSpeed - powerDifference, CW, maxSpeed);
    motorL.set_input(maxSpeed - powerDifference);
    motorR.set_input(maxSpeed);
  } else {
    // set_motor(CCW, maxSpeed, CW, maxSpeed + powerDifference);
    motorL.set_input(maxSpeed);
    motorR.set_input(maxSpeed + powerDifference);
  }
}

// Count the sensor bits that is ON 
int count_on() {
  count1 = 0;
  // Sensor has 16 bits of output - LOW is ON - HIGH is OFF
  for (int i = 2; i < 14; i++) {
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
  for (int i = 2; i < 14; i++) {
    input_array[i] = 1 - digitalRead(input_pin[i]);
  }
  position_value = 0;
  int numer = 0;
  int denom = 0;
  for (int i = 2; i < 14; i++) {
    numer += input_array[i] * sensorweight[i]; //  Calculates the position of the sensor array over the line by weighted average method. The weights
    denom += input_array[i]; //  are assigned as per sensorWeight[]
  }  
  position_value = 550 - (numer / denom); // Leftmost is -750 and rightmost is 750
  return position_value;
}