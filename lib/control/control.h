#include "GPIO.h"
void straight();
void back();
void right();
void left();
void stopp();
void set_motor (int direction_left, int speed_left, int direction_right, int speed_right);
void rotate (int side);
void check();
unsigned int data_value();

void check() {
  bitWrite(state, 0, digitalRead(3));
  bitWrite(state, 1, digitalRead(6));
  bitWrite(state, 2, digitalRead(7));
}

void straight() {
  // Serial.println("straight");
  digitalWrite(FR1, 1);
  analogWrite(SV1, 50);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 50);
}

void back() {
  // Serial.println("back");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 80);
  digitalWrite(FR2, 1);
  analogWrite(SV2, 80);
}

//Function that make the robot turn right
void right() {
  // Serial.println("right");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 80);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 80);
}

//Fucntion that make the robot turn left
void left() {
  // Serial.println("left");
  digitalWrite(FR1, 1);
  analogWrite(SV1, 80);
  digitalWrite(FR2, 1);
  analogWrite(SV2, 80);
}

//Function that make the robot stopp
void stopp() {
  // Serial.println("stop");
  digitalWrite(FR1, 0);
  analogWrite(SV1, 0);
  digitalWrite(FR2, 0);
  analogWrite(SV2, 0);
}

void set_motor(int direction_left, int speed_left, int direction_right, int speed_right) {
  digitalWrite(FR1, direction_left);
  analogWrite(SV1, speed_left);
  digitalWrite(FR2, direction_right);
  analogWrite(SV2, speed_right);
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