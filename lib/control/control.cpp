#include "control.h"
float speed_left = 0;
float speed_right = 0;
int state = 0;

void check() {
  bitWrite(state, 0, digitalRead(3));
  bitWrite(state, 1, digitalRead(6));
  bitWrite(state, 2, digitalRead(7));
}

//Function that make the robot goes straight
void straight() {
  speed_left = speed_right = 0.034/0.125*255; // m/s
  set_motor(0, speed_left, 1, speed_right);
}

//Function that make the robot goes backward
void back() {
  speed_left = speed_right = 255; // m/s
  set_motor(1, speed_left, 0, speed_right);
}

//Function that make the robot rotates right
void right() {
  speed_left = speed_right = 0.06;
  set_motor(0, speed_left, 0, speed_right);
}

//Fucntion that make the robot turns left
void left() {
  speed_left = speed_right = 0.06;
  set_motor(1, speed_left, 1, speed_right);
}

//Function that make the robot stops
void stopp() {
speed_left = speed_right = 0;
  set_motor(1, speed_left, 0, speed_right);
}

// Setting up the input direction and speed of two motors
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right) {
  // float speedL = speed_left/0.125*255;
  // float speedR = speed_right/0.125*255;
  if(speed_left > 255)
  {
    speed_left = 255;
  }
  if(speed_right > 255)
  {
    speed_right = 255;
  }
  digitalWrite(FR1, direction_right);
  analogWrite(SV1, speed_right);
  digitalWrite(FR2, direction_left);
  analogWrite(SV2, speed_left);
}


// unsigned int data_value() {
//   unsigned int input_value = 0b0000000000000000;
//   for (int i = 0; i < 16; i++) {
//     bitWrite(input_value, i, 1 - digitalRead(input_pin[i]));
//   }
//   return input_value;
// }

// void rotate(int side) {
//   straight();
//   delay(2000);
//   stopp();
//   while (data_value() != 0b0000001111000000) { //May have to check middle 4 sensors if there is loop in track

//     //Perform rotation
//     if (side == RIGHT) {
//       set_motor(0, rotateSpeed, 0, rotateSpeed);
//     } else if (side == LEFT) {
//       set_motor(1, rotateSpeed, 1, rotateSpeed);
//     }
//   }}