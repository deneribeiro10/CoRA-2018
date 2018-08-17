#include "C:\Users\DELL\Desktop\Coltec 2018\CORA\CoRA-2018\Follow\lib\Robot.h"

#define KP 255
#define VEL_MAX 255

enum state {
  waiting,
  follow
};

Robot *sheep = new Robot();
int sensors[] = {0,1,2,3,4};
int btn = 12;
bool button_last = false;
state robot;
int follow_state;

void setup() {
  Serial.begin(9600);
  robot = waiting;
  pinMode(btn, INPUT);
  sheep->create(); 
}

void loop() {
  bool button;
  bool s[5];

  switch(robot) {
    case waiting: 
      button = digitalRead(btn);
      if(button == false && button_last == true) {
        robot = follow;
      }
      button_last = button;
      break;
      
    case follow:
      for(int i=0; i<5; i++) {
        s[i] = analogRead( sensors[i] ) < 150;
      }
      follow_state = getState(s, follow_state);
      if(follow_state > 0) {
        sheep->stopMotorLeft();
        sheep->startMotorRight(VEL_MAX);
      } else if(follow_state < 0) {
        sheep->startMotorLeft(VEL_MAX);
        sheep->stopMotorRight();
      }
      break;
  }
}

int getState(bool s[], int stAnt) {
    int st = 0;
    if(s[1]) {
      st = 2;
      if(s[2])
        st--;
    } else if(s[3]) {
      st = -2;
      if(s[2])
        st++;
    } else if(stAnt == 2){
      st = 3;
    } else if(stAnt == -2){
      st = -3;
    }
    return st;
  }

