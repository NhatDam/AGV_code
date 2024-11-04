#ifndef AGV_CONTROL_H
#define AGV_CONTROL_H
#include "GPIO.h"

#define CW 1
#define CCW 0
// Variables and functions declaration
void straight(float speed_left, float speed_right);
void back(float speed_left, float speed_right);
void right(float speed_left, float speed_right);
void left(float speed_left, float speed_right);
void stopp();
extern unsigned char reverse_L;
extern unsigned char reverse_R;
void setMotorSpeeds(int leftSpeed, int rightSpeed);
void setEachMotorSpeed(int wheel, int spd);
// void handle_motor_com(float speed_left, float speed_right);
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right);
// void check();
// void rotate (int side);
// unsigned int data_value();

#endif