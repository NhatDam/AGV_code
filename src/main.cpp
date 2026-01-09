#include <Arduino.h>
#include "Follow_line.hpp"
#include "Speed_read.hpp"
#include "GPIO.h"
#include "SoC.h"
#include "../../include/commands.h"
#include"Esp_control.hpp"
#include "Safe.hpp"

// Set PID rate as 30 times per loop
#define PID_rate 30
#define PID_interval 1000/PID_rate
#define safe_interval 100







unsigned long next_PID = 0;
unsigned long next_safety = 0;



long t, t_prev = 0;
float deltaT = 0;
long lastTime = 0;

// // Constants
// const float targetRPM = 100.0;
// const float testDuration = 5.0; // seconds
// const float sampleTime = 0.1;   // seconds
// const int samples = testDuration / sampleTime;



// // Ranges to search (adjust for finer tuning)
// float Kp_vals[] = {2, 3, 4};
// float Ki_vals[] = {0};
// float Kd_vals[] = {0};





void setup() {
  Serial.begin(57600);
  Serial2.begin(115200);

  //Declare output pin for control UVC light
  pinMode(15, OUTPUT);
  // battery_init();
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);

  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses,RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(42, INPUT);
  pinMode(48, INPUT);
  pinMode(47, INPUT);
  pinMode(43, INPUT);
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

  UVC(t);
  SAFE();
  ESP_Control(t);
  if(isFollowLine){
    moving = 1;
    follow_line(t);
  }

  // SAFE();
  
    
  if (millis() > next_PID) {
    deltaT = ((double)(t - t_prev)) / 1.0e6;  // convert to seconds
    t_prev = t;
    local_RPM(deltaT);
    // Serial.print(get_speed_rad_per_sec(LEFT)); 
    // Serial.print(" ");
    // Serial.println(get_speed_rad_per_sec(RIGHT));
    if(!agv_halted){
      motorL.do_PID();
      motorR.do_PID();
    }
    next_PID += PID_interval;
  }
  // for(int i = 0; i < 16; i++){
  //   Serial.print(marked_plant[i]);
  //   Serial.print(" ");
  // }
  // Serial.println(" ");
}
