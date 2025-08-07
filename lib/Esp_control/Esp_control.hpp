#include "Follow_line.hpp"

#define AUTO_STOP_INTERVAL 5000 //2000
extern boolean isFollowLine;
extern long lastMotorCommand; 
void ESP_Control(long t);