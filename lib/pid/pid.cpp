#include "pid.hpp"
#include <math.h>

PID_CLASS::PID_CLASS(double kp, double kd, double ki, int motor) : Kp(kp), Ki(ki), Kd(kd), Motor(motor) {}
boolean status = 0;
boolean agv_halted = false;
void PID_CLASS::calculate()
{
  // Read the feedback data of motor current speed (RPM)
  switch (Motor)
  {
  case LEFT:
    this->actualSpeed = speed_left;
    break;
  case RIGHT:
    this->actualSpeed = speed_right;
    break;
  }
  // Sometimes integration is NAN
  if (isnan(integration))
  {
    integration = 0;
  }
  // Calculate error
  error = inputSpeed - actualSpeed;
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-tuning-changes/
  integration = integration + Ki * error;
  // http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-derivative-kick/
  double dInput = actualSpeed - last_actual_speed;

  // PID calculation
  // Define max RPM and max PWM (based on your test)
  u = (Kp * error - Kd * dInput + integration);
  // if (status == 1)
  // {
  //   u = -y;
  // }
  // else {
  //   u = y;
  // }
  last_actual_speed = actualSpeed;

  // Drive the motor using PID outputs
  switch (Motor)
  {
  case 0:
    setEachMotorSpeed(LEFT, u);
    break;
  default:
    setEachMotorSpeed(RIGHT, u);
    break;
  }
}

// Set the target speed to do PID
void PID_CLASS::set_input(double input)
{
  this->inputSpeed = input;
  // status = 0;
  //   if (input < 0)
  // {
  //   inputSpeed = -inputSpeed;
  //   status = 1;
  // }
}

// Print the output of PID
double PID_CLASS::get_output()
{
  return this->u;
}

// Reset all PID related variables
void PID_CLASS::reset_PID()
{
  this->inputSpeed = 0;
  this->error_prev = 0;
  this->actualSpeed = 0;
  this->last_actual_speed = 0;
  this->u = 0;
  this->integration = 0;
}

// Set new PID parameters - Kp Kd Ki
void PID_CLASS::set_PID(double kp, double kd, double ki)
{
  this->Kp = kp;
  this->Kd = kd;
  this->Ki = ki;
}

// Conduct PID calculation process
void PID_CLASS::do_PID()
{
  if (!moving)
  {

    if (this->last_actual_speed != 0)
      this->reset_PID();
    return;
  }

  this->calculate();
}

void agvResume()
{
  // Serial.println("Resume");
  agv_halted = false;
}

void agvHalt()
{

  // Stop PWM output
  stopp();
  // Reset PID integrators
  motorL.reset_PID();
  motorR.reset_PID();

  // Set target RPM to zero
  motorL.set_input(0);
  motorR.set_input(0);

  // Optional: set a "halted" flag to block PID updates
  agv_halted = true;
}