#include "GPIO.h"
extern float speed_left ;
extern float speed_right ;

void straight();
void back();
void right();
void left();
void stopp();
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right);
void rotate (int side);
void check();
unsigned int data_value();

