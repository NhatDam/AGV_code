#include "Esp_control.hpp"
/* Stop the robot if it hasn't received a movement command
   in this number of milliseconds */
boolean isFollowLine = false;
long lastMotorCommand = AUTO_STOP_INTERVAL;
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
void runCommand(long t) {
  // int i = 0;
  // char *p = argv1;
  // char *str;
  // double pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  switch(cmd) {
    // Set motor speeds terminal command
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    //lastMotorCommand = millis();
    isFollowLine = false;
    if (arg1 == 0 && arg2 == 0) {
      setMotorSpeeds(0, 0);
      motorL.reset_PID();
      motorR.reset_PID();
      moving = 0;
      // Serial.println("STOP");
    }
    else {
      moving = 1;
      motorL.set_input(arg1);
      motorR.set_input(arg2);
      // Serial.print("MOVE ");
      // Serial.print(arg1);
      // Serial.print(" ");
      // Serial.println(arg2);
    }
    break;
  case AUTOMATED:
    isFollowLine = true;
    // Serial.println("AUTOMATED ON");
    break;
  case MARK_PLANT:
    int i = arg1-1;
    if(marked_plant[i] == 0){
      marked_plant[i] = 1;
    }else{
      marked_plant[i] = 0;
    }
    break;
  case RESET_PLANT:
    count_plant = 0;
  default:
    break;
  }
}

void ESP_Control(long t){
    while (Serial2.available() > 0) {
    
    // Read the next character
    chr = Serial2.read();

    // Terminate a command with a CR
    if (chr == '\'') {
      if (arg == 1) argv1[index] = '\0';
      else if (arg == 2) argv2[index] = '\0';
      runCommand(t);
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
}