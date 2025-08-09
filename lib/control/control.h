#ifndef AGV_CONTROL_H
#define AGV_CONTROL_H
#include "GPIO.h"

// Variables and functions declaration
extern float speed_left ;
extern float speed_right ;
void straight(float speed);
void back(float speed);
void right(float speed);
void left(float speed);
void stopp();
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right);
// void check();
// void rotate (int side);
// unsigned int data_value();

#endif