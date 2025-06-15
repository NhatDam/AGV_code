#include <Arduino.h>
#include "Follow_line.hpp"
#include "Speed_read.hpp"
#include "pid.hpp"
#include "GPIO.h"
#include "control.hpp"
#include "SoC.h"
#include "../../include/commands.h"



// Set PID rate as 30 times per loop
#define PID_rate 30
#define PID_interval 1000/PID_rate

PID_CLASS motorL(1, 0, 0, LEFT); 
PID_CLASS motorR(1, 0, 0, RIGHT);

/* Stop the robot if it hasn't received a movement command
   in this number of milliseconds */
#define AUTO_STOP_INTERVAL 5000 //2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

unsigned long next_PID = 0;
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

long t, t_prev = 0;
float deltaT = 0;
long lastTime = 0;

// Constants
const float targetRPM = 100.0;
const float testDuration = 5.0; // seconds
const float sampleTime = 0.1;   // seconds
const int samples = testDuration / sampleTime;

PID_CLASS pid(1, 0, 0, RIGHT);

// Ranges to search (adjust for finer tuning)
float Kp_vals[] = {2, 3, 4};
float Ki_vals[] = {0};
float Kd_vals[] = {0};

void autoTunePID() {
  float bestError = 999999;
  float bestKp = 0, bestKi = 0, bestKd = 0;

  Serial.println("Starting auto-tune...");

  // Kickstart the motor
  moving = 1;
  pid.set_input(targetRPM);
  for (float Kp : Kp_vals) {
    for (float Ki : Ki_vals) {
      for (float Kd : Kd_vals) {
        pid.set_PID(Kp, Kd, Ki);
        pid.reset_PID();
        
        
        float totalError = 0;

        for (int i = 0; i < samples; i++) {
          local_RPM(sampleTime);
          pid.do_PID();
          delay(sampleTime * 1000);

          float currentRPM = get_speed_rpm(LEFT);
          float error = abs(currentRPM - targetRPM);
          totalError += error;
        }

        Serial.print("Kp: "); Serial.print(Kp);
        Serial.print(", Ki: "); Serial.print(Ki);
        Serial.print(", Kd: "); Serial.print(Kd);
        Serial.print(", Error: "); Serial.println(totalError);

        set_motor(CCW, 0, CW, 0);
        delay(1000);

        if (totalError < bestError) {
          bestError = totalError;
          bestKp = Kp;
          bestKi = Ki;
          bestKd = Kd;
        }
      }
    }
  }

  Serial.println("Best tuning:");
  Serial.print("Kp: "); Serial.println(bestKp);
  Serial.print("Ki: "); Serial.println(bestKi);
  Serial.print("Kd: "); Serial.println(bestKd);
}


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
  double pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
  case 't':
    moving = 1;
    autoTunePID(); // Start auto-tuning
    moving = 0;
  break;
  // Read encoder terminal command
  case READ_ENCODERS:
    Serial.print("Count L: "); Serial.print(countL_i);
    Serial.print(", Count R: "); Serial.println(countR_i);
    Serial.print("Raw Speed L (p/s): "); Serial.print(speed_left);
    Serial.print(", R: "); Serial.println(speed_right);
    Serial.print("RPM L: "); Serial.print(get_speed_rpm(LEFT));
    Serial.print(", R: "); Serial.println(get_speed_rpm(RIGHT));
  break;
  case RESET_ENCODERS:
    reset_encoder(LEFT);
    reset_encoder(RIGHT);
    motorL.reset_PID();
    motorR.reset_PID();
    Serial.println("OK");
  break;
    // Set motor speeds terminal command
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      motorL.reset_PID();
      motorR.reset_PID();
      moving = 0;
    }
    else moving = 1;
    motorL.set_input(arg1);
    motorR.set_input(arg2);
    Serial.println("OK"); 
  break;

  case MOTOR_RAW_PWM:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    motorL.reset_PID();
    motorR.reset_PID();
    moving = 0; // Sneaky way to temporarily disable the PID
    setMotorSpeeds(arg1, arg2);
    Serial.println("OK"); 
  break;

  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != NULL) {
      if(i < 3) {
          pid_args[i] = atof(str);
          switch(i) {
            case 0:
              Serial.print("Kp: ");
              Serial.println(pid_args[i]);
            break;
            case 1:
              Serial.print("Kd: ");
              Serial.println(pid_args[i]);
            break;
            case 2:
              Serial.print("Ki: ");
              Serial.println(pid_args[i]);
          }
          i++;
        }
    }
    motorL.set_PID(pid_args[0], pid_args[1], pid_args[2]);
    motorR.set_PID(pid_args[0], pid_args[1], pid_args[2]);
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
  motorL.reset_PID();
  motorR.reset_PID();
}


void loop() {
  // Get current time in uS
  t = micros();  

  
  check();
  

  while (Serial.available() > 0) {
    
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == '\'') {
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

  // Do PID for all motors with fixed interval
  if (millis() > next_PID) {
    deltaT = ((double)(t - t_prev)) / 1.0e6;  // convert to seconds
    t_prev = t;
    local_RPM(deltaT);
    motorL.do_PID();
    motorR.do_PID();
    next_PID += PID_interval;
  
  }
  // switch (state) {
  // case 7:
  //   stopp();
  //   break;
  // case 6:
  //   // Serial.println("Straight");
  //   straight(120,120);
  //   break;
  // case 5:
  //   // Serial.println("Back");
  //   back(100,100);
  //   break;
  // case 4:
  //   // Serial.println("Left");
  //   left(70,70);
  //   break;
  // case 3:
  //   // Serial.println("Right");
  //   right(70,70);
  //   break;
  // case 0:
  //   follow_line();
  //   break;
  // default:
  //   stopp();
  //   break;
  // }

  
}
