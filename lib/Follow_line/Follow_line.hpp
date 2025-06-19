#ifndef AGV_LINEFOLLOW_H
#define AGV_LINEFOLLOW_H
#include "control.hpp"
#include "pid.hpp"
// Function declarations
void follow_line();
void PID ();
int count_on ();
int sensor_position();

extern PID_CLASS motorL; 
extern PID_CLASS motorR;
#endif
