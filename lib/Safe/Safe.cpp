#include "Safe.hpp"

void SAFE(){
  if (!digitalRead(43) || !digitalRead(48) || !digitalRead(47) || !digitalRead(42))
  {
    // Serial.println("Halted safe");
    agvHalt();
  }
  else if(!light_on){
    agvResume();
  }
}