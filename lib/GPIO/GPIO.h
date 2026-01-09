#ifndef AGV_GPIO_H
#define AGV_GPIO_H

#include "Arduino.h"
// 1 is right and 2 is left looking at the AGV rear
extern int FR1;
extern int FR2;
extern int SV1 ;
extern int SV2 ;

extern int state;
extern int input_pin[]; // all the input pin connect to the sensor
extern int input_array[];
extern byte input_value; // store the input value                                                            
extern int count1;
extern const int sensorweight[];
extern int position_value;
extern int error, lastError;
extern int maxSpeed;
extern int rotateSpeed;

#endif
