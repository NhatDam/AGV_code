#include "control.h"
float speed_left = 0;
float speed_right = 0;
int state = 0;

// void check() {
//   bitWrite(state, 0, digitalRead(3));
//   bitWrite(state, 1, digitalRead(6));
//   bitWrite(state, 2, digitalRead(7));
// }

//Function that make the robot goes straight with specified velocity in m/s
void straight(float speed) {
  speed_left = speed_right = speed/0.125*255; // m/s
  set_motor(0, speed_left, 1, speed_right);
}

//Function that make the robot goes backward
void back(float speed) {
  speed_left = speed_right = speed/0.125*255; // m/s
  set_motor(1, speed_left, 0, speed_right);
}

//Function that make the robot rotates right
void right(float speed) {
  speed_left = speed_right = speed/0.125*255;
  set_motor(0, speed_left, 0, speed_right);
}

//Fucntion that make the robot turns left
void left(float speed) {
  speed_left = speed_right = speed/0.125*255;
  set_motor(1, speed_left, 1, speed_right);
}

//Function that make the robot stops
void stopp() {
speed_left = speed_right = 0;
  set_motor(1, speed_left, 0, speed_right);
}

// Setting up the input direction and speed of two motors
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right) {
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
