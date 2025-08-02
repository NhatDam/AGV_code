#include "Esp_control.hpp"
int state;

void check() {
  bitWrite(state, 0, digitalRead(7));
  bitWrite(state, 1, digitalRead(8));
  bitWrite(state, 2, digitalRead(9));
}

void ESP_Control(long t){
    check();
    switch (state) {
  case 7:
    moving = 0;
    stopp();
    break;
  case 6:
    // Serial.println("Straight");
    // straight(120,120);
    moving = 1;
    motorL.set_input(jetson_input);
    motorR.set_input(jetson_input);
    break;
  case 5:
    // Serial.println("Back");
    // back(100,100);
    moving = 1;
    motorL.set_input(-jetson_input);
    motorR.set_input(-jetson_input);
    break;
  case 4:
    // Serial.println("Left");
    // left(70,70);
    moving = 1;
    motorL.set_input(-jetson_input);
    motorR.set_input(jetson_input);
    break;
  case 3:
    // Serial.println("Right");
    // right(70,70);
    moving = 1;
    motorL.set_input(jetson_input);
    motorR.set_input(-jetson_input);
    break;
  case 0:
    moving = 1;
    follow_line(t);
    break;
  default:
    moving = 0;
    stopp();
    break;
  }
}