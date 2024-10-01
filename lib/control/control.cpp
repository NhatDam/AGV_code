#include "control.h"
float speed_left = 0;
float speed_right =0;
int state = 0;
void check() {
  bitWrite(state, 0, digitalRead(3));
  bitWrite(state, 1, digitalRead(6));
  bitWrite(state, 2, digitalRead(7));
}

void straight() {
  speed_left = speed_right = 255;
  set_motor(0, speed_left, 1, speed_right);
}

void back() {
  speed_left = speed_right = 80;
  set_motor(1, speed_left, 0, speed_right);
}

//Function that make the robot rotate right
void right() {
  speed_left = speed_right = 80;
  set_motor(0, speed_left, 0, speed_right);
}

//Fucntion that make the robot turn left
void left() {
  speed_left = speed_right = 80;
  set_motor(1, speed_left, 1, speed_right);
}

//Function that make the robot stopp
void stopp() {
speed_left = speed_right = 0;
  set_motor(1, speed_left, 0, speed_right);
}

void set_motor (int direction_left, float speed_left, int direction_right, float speed_right) {
  digitalWrite(FR1, direction_right);
  analogWrite(SV1, speed_right);
  digitalWrite(FR2, direction_left);
  analogWrite(SV2, speed_left);
}

unsigned int data_value() {
  unsigned int input_value = 0b0000000000000000;
  for (int i = 0; i < 16; i++) {
    bitWrite(input_value, i, 1 - digitalRead(input_pin[i]));
  }
  return input_value;
}

void rotate(int side) {
  straight();
  delay(2000);
  stopp();
  while (data_value() != 0b0000001111000000) { //May have to check middle 4 sensors if there is loop in track

    //Perform rotation
    if (side == RIGHT) {
      set_motor(0, rotateSpeed, 0, rotateSpeed);
    } else if (side == LEFT) {
      set_motor(1, rotateSpeed, 1, rotateSpeed);
    }
  }
}