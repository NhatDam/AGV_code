#include "control.h"
int state = 0;

// void check() {
//   bitWrite(state, 0, digitalRead(3));
//   bitWrite(state, 1, digitalRead(6));
//   bitWrite(state, 2, digitalRead(7));
// }

//Function that make the robot goes straight with specified velocity in m/s
void straight(float speed_left, float speed_right) {
  set_motor(CCW, speed_left/0.125*255, CW, speed_right/0.125*255);
}

//Function that make the robot goes backward
void back(float speed_left, float speed_right) {
  
  set_motor(CW, speed_left/0.125*255, CCW, speed_right/0.125*255);
}

//Function that make the robot rotates right
void right(float speed_left, float speed_right) {
  
  set_motor(CCW, speed_left/0.125*255, CCW, speed_right/0.125*255);
}

//Fucntion that make the robot turns left
void left(float speed_left, float speed_right) {
  set_motor(CW, speed_left/0.125*255, CW, speed_right/0.125*255);
}

//Function that make the robot stops
void stopp() {
  set_motor(0, 0, 0, 0);
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

void handle_motor_com(float speed_left, float speed_right){
  if(speed_left > 0 && speed_right > 0)
  {
    straight(speed_left,speed_right);
  }
  else if(speed_left < 0 && speed_right < 0)
  {
    back(speed_left,speed_right);
  }
  else if(speed_left < 0 && speed_right > 0)
  {
    left(speed_left,speed_right);
  }
  else if (speed_left > 0 && speed_right < 0){
    right(speed_left,speed_right);
  }
  else{
    stopp();
  }
}
