#ifndef PID_HPP
#define PID_HPP

#include <Arduino.h>
#include "control.hpp"
#include "Speed_read.hpp"

#define MOTOR1 1
#define MOTOR2 2
class PID_CLASS {
public:
    // PID gains
    double Kp;
    double Ki;
    double Kd;
    // Other PID parameters
    double actualSpeed {0.0};
    double integration {0.0};
    double derivative {0.0};
    double last_actual_speed;
    int Motor = 0;
public:
    double inputSpeed {0.0};
    double error = 0, error_prev = 0;
    double y {0};
    double u {0};
    PID_CLASS(double kp, double kd, double ki, int motor); // Constructor
    void calculate(); // Calculate PID output
    void set_input(double input);
    double get_output();
    void reset_PID();
    void do_PID();
    void set_PID(double kp, double kd, double ki);
};

#endif // PID_HPP