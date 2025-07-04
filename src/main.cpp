
/*********************************************************************
 *  ROSArduinoBridge
 
    A set of simple serial commands to control a differential drive
    robot and receive back sensor and odometry data. Default 
    configuration assumes use of an Arduino Mega + Pololu motor
    controller shield + Robogaia Mega Encoder shield.  Edit the
    readEncoder() and setMotorSpeed() wrapper functions if using 
    different motor controller or encoder method.

    Created for the Pi Robot Project: http://www.pirobot.org
    and the Home Brew Robotics Club (HBRC): http://hbrobotics.org
    
    Authors: Patrick Goebel, James Nugen

    Inspired and modeled after the ArbotiX driver by Michael Ferguson
    
    Software License Agreement (BSD License)

    Copyright (c) 2012, Patrick Goebel.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials provided
       with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#include "Follow_line.h"
#include "Speed_read.h"
#include "GPIO.h"
#include "control.h"
#include "SoC.h"
#include "commands.h"
#include "AGV_controller.h"

DFRobot_INA219_IIC ina219(&Wire,INA219_I2C_ADDRESS4);
// Revise the following two paramters according to actula reading of the INA219 and the multimeter
// for linearly calibration
float ina219Reading_mA = 1920;
float extMeterReading_mA = 1900;

/* Run the PID loop at 30 times per second */
#define PID_RATE           30     // Hz

/* Convert the rate into an interval */
const int PID_INTERVAL = 1000 / PID_RATE;

/* Track the next time we make a PID calculation */
unsigned long nextPID = PID_INTERVAL;

/* Stop the robot if it hasn't received a movement command
   in this number of milliseconds */
#define AUTO_STOP_INTERVAL 5000 //2000
long lastMotorCommand = AUTO_STOP_INTERVAL;
void check() {
  bitWrite(state, 0, digitalRead(7));
  bitWrite(state, 1, digitalRead(8));
  bitWrite(state, 2, digitalRead(9));
}
int state;
// A pair of varibles to help parse serial commands 
int arg = 0;
int index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;

unsigned long t, tprev = 0;
float deltaT = 0;
long lastTime = 0;

/* Clear the current command parameters */
void resetCommand() {
  cmd = '\0';
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

/* Run a command.  Commands are defined in commands.h */
void runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
  // Read encoder terminal command
  case READ_ENCODERS:
    Serial.print(read_encoder(LEFT));
    Serial.print(" ");
    Serial.println(read_encoder(RIGHT));
    Serial.println(reverse_L);
    Serial.print(" ");
    Serial.println(reverse_R);
    break;
  case RESET_ENCODERS:
  reset_encoder(LEFT);
  reset_encoder(RIGHT);
  resetPID();
  Serial.println("OK");
  break;
  // Set motor speeds terminal command
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      resetPID();
      moving = 0;
    }
    else moving = 1;
    leftPID.TargetCountPerLoop = arg1;
    rightPID.TargetCountPerLoop = arg2;
    Serial.println("OK"); 
    break;

  case MOTOR_RAW_PWM:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    setMotorSpeeds(arg1, arg2);
    Serial.println("OK"); 
    break;

  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != NULL) {
       pid_args[i] = atoi(str);
       i++;
    }
    Kp = pid_args[0];
    Kd = pid_args[1];
    Ki = pid_args[2];
    Serial.println("OK");
    break;
  default:
    Serial.println("Invalid Command");
    break;
  }
}


void setup() {
  Serial.begin(57600);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  // battery_init();
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);

  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses,RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  ina219.begin();
  ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
  resetPID();
}


void loop() {
  // Get current time in uS
  t = micros();  
  // Calculating elapsed time deltaT in S
  deltaT = ((float)(t - tprev))/1.0e6;
  tprev=t;
  // //calculate RPM by mega
  // local_RPM(deltaT);
  check();
   
  // Serial.print("Left: ");
  // Serial.println(speed_left);
  // Serial.print("Right: ");
  // Serial.println(speed_right);
  // delay(500);
  
  
 

  while (Serial.available() > 0) {
    
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == '\r') {
      if (arg == 1) argv1[index] = '\0';
      else if (arg == 2) argv2[index] = '\0';
      runCommand();
      resetCommand();
    }
    // Use spaces to delimit parts of the command
    else if (chr == ' ') {
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = '\0';
        arg = 2;
        index = 0;
      }
      continue;
    }
    else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index] = chr;
        index++;
      }
      else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }
  // //Run PID calculation at the approriate intervals
  // if (millis() > nextPID) {
  //   updatePID();
  //   nextPID += PID_INTERVAL;
  // }
  // Check to see if we have exceeded the auto-stop interval
  // if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {;
    //setMotorSpeeds(0, 0);
  switch (state) {
  case 7:
    stopp();
    break;
  case 6:
    // Serial.println("Straight");
    straight(120,120);
    break;
  case 5:
    // Serial.println("Back");
    back(100,100);
    break;
  case 4:
    // Serial.println("Left");
    left(70,70);
    break;
  case 3:
    // Serial.println("Right");
    right(70,70);
    break;
  case 0:
    follow_line();
    break;
  default:
    stopp();
    break;
  }
  //   moving = 0;
  // }
  // Calculate_V_and_A(ina219);
  // Serial.print(voltage); Serial.print(",");

  // Serial.print(current); Serial.print(",");

  // Serial.print(speed_left); Serial.print(",");

  // Serial.print(speed_right); Serial.print(",");

  // Serial.print(0); Serial.println();
  

}
