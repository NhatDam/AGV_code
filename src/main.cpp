#include <Follow_line.h>
//Declare variables for speed reading
const int speedPinleft = 20; 
const int speedPinright = 21; 
volatile unsigned int pulseCountleft = 0;
volatile unsigned int pulseCountright = 0;
unsigned long lastTime = 0;
unsigned int pulsesPerRevolution = 5;

// Calculate RPM every second
void print_RPM(unsigned long currentTime){
  if (currentTime - lastTime >= 1000) {
    noInterrupts(); // Disable interrupts temporarily to read pulseCount
    unsigned int countleft = pulseCountleft;
    unsigned int countright = pulseCountright;
    pulseCountleft = 0; // Reset pulse count
    pulseCountright = 0;
    interrupts(); // Re-enable interrupts

    // Calculate RPM
    float LeftRPM = (countleft / (float)pulsesPerRevolution) * 60.0;
    float RightRPM = (countright / (float)pulsesPerRevolution) * 60.0;
    
    // Print RPM values
    Serial.print("Left Motor RPM: ");
    Serial.println(LeftRPM);
    Serial.print("Right Motor RPM: ");
    Serial.println(RightRPM);
    
    // Update the lastTime
    lastTime = currentTime;
  }
}

void countLeftPulses() {
  pulseCountleft++;
};

void countRightPulses() {
  pulseCountright++;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int a = 0; a < 16; a++) {
    pinMode(input_pin[a], INPUT_PULLUP);
  }

  // Speed signal inputs
  pinMode(speedPinleft, INPUT);
  pinMode(speedPinright, INPUT);
  // Attach interrupts for both motors
  attachInterrupt(digitalPinToInterrupt(speedPinleft), countLeftPulses, RISING);
  attachInterrupt(digitalPinToInterrupt(speedPinright), countRightPulses, RISING);

  pinMode(FR1, OUTPUT);
  pinMode(FR2, OUTPUT);
  pinMode(SV1, OUTPUT);
  pinMode(SV2, OUTPUT);
  pinMode(3, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(A1,INPUT);
}

void loop() {
  // check();
  
  unsigned long currentTime = millis();
  print_RPM(currentTime);
  switch (6) {
  case 7:
    stopp();
    break;
  case 6:
    straight();
    break;
  case 5:
    back();
    break;
  case 4:
    left();
    break;
  case 3:
    right();
    break;
  case 0:
    follow_line();
    break;
  default:
    stopp();
    break;
  }
}








