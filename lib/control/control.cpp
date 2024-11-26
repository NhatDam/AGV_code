#include "control.h"
#include "D:/RMIT/AGV_Project_2024/AGV_code/AGV_code/include/commands.h"
unsigned char reverse_L;
unsigned char reverse_R;
//Function that make the robot goes straight with specified velocity in m/s
void straight(float speed_left, float speed_right) {
  set_motor(CCW, speed_left, CW, speed_right);
}

//Function that make the robot goes backward
void back(float speed_left, float speed_right) {
  
  set_motor(CW, speed_left, CCW, speed_right);
}

//Function that make the robot rotates right
void right(float speed_left, float speed_right) {
  
  set_motor(CCW, speed_left, CCW, speed_right);
}

//Fucntion that make the robot turns left
void left(float speed_left, float speed_right) {
  set_motor(CW, speed_left, CW, speed_right);
}

//Function that make the robot stops
void stopp() {
  set_motor(0, 0, 0, 0);
}
void initMotorController()
{

}
void setEachMotorSpeed(int wheel, int spd) {
  if(wheel == LEFT) reverse_L = 0;
  else if(wheel == RIGHT) reverse_R = 0;
    if (spd < 0)
    {
      spd = -spd;
      if(wheel == LEFT) reverse_L = 1;
      else if(wheel == RIGHT) reverse_R = 1;
    }
    if (spd > 255)
      spd = 255;
    
    if (wheel == LEFT) { 
      if      (reverse_L == 0) { digitalWrite(FR2, CCW);analogWrite(SV2, spd); }
      else if (reverse_L == 1) { digitalWrite(FR2, CW);analogWrite(SV2, spd); }
    }
    else /*if (wheel == RIGHT) //no need for condition*/ {
      if      (reverse_R == 0) { digitalWrite(FR1, CW);analogWrite(SV1, spd); }
      else if (reverse_R == 1) { digitalWrite(FR1, CCW);analogWrite(SV1, spd); }
    }
  }
// Setting up the input direction and speed of two motors for Magnetic following line
void set_motor (int direction_left, float speed_left, int direction_right, float speed_right) {
  // Limit the speed command signal as 255
  if(speed_left > 255)
  {
    speed_left = 255;
  }
  if(speed_right > 255)
  {
    speed_right = 255;
  }

  // Check the direction of left wheel
  if(direction_left == CCW)
  {
    reverse_L = 0;
  }
  else
  {
    reverse_L = 1;
  }
   // Check the direction of right wheel
  if(direction_right == CW)
  {
    reverse_R = 0;
  }
  else
  {
    reverse_R = 1;
  }
  // Drive the motors
  digitalWrite(FR1, direction_right);
  digitalWrite(FR2, direction_left);
  analogWrite(SV1, speed_right);
  analogWrite(SV2, speed_left);
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setEachMotorSpeed(LEFT, leftSpeed);
    setEachMotorSpeed(RIGHT, rightSpeed);
  }

// void handle_motor_com(float speed_left, float speed_right){
//   if(speed_left > 0 && speed_right > 0)
//   {
//     straight(speed_left,speed_right);
//   }
//   else if(speed_left < 0 && speed_right < 0)
//   {
//     back(-speed_left,-speed_right);
//   }
//   else if(speed_left < 0 && speed_right > 0)
//   {
//     left(-speed_left,speed_right);
//   }
//   else if (speed_left > 0 && speed_right < 0){
//     right(speed_left,-speed_right);
//   }
//   else{
//     stopp();
//   }
// }
