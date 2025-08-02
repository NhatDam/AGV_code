#ifndef AGV_LINEFOLLOW_H
#define AGV_LINEFOLLOW_H
#include "pid.hpp"

// Function declarations
void follow_line(long t);
void PID ();
void UVC(long time);
int count_on ();

int sensor_position();
extern boolean flag1;

extern int count_plant;
extern int prev_detected; // Added to track previous state of marker detection
extern int input_array2[16];
extern int marked_plant[8];  
extern boolean light_on;

#endif
