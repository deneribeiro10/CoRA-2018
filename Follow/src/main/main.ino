#include "C:\Users\DELL\Desktop\Coltec 2018\CORA\CoRA-2018\Follow\lib\Robot.h"

Robot *sheep = new Robot();

void setup() {
  Serial.begin(9600);
  
  sheep->create(); 
  
}

void loop() {
  int s1 = analogRead(2);
  int s2 = analogRead(3);

  if(s1 >= 512) {
    sheep->startMotorRight(150);
  } else {
    sheep->stopMotorRight();
  }
  if(s2 >= 512) {
    sheep->startMotorLeft(150);
  } else {
    sheep->stopMotorLeft();
  }
}

