#include "lib/Robot.h"

#define VEL_MAX 254

Robot *sheep = new Robot();
int erro;
int last_erro = 0;
int sqrCount[2] = {0, 0};
bool* s;
bool s_last[7];
bool inversao = false;

void setup() {
  counterReset();
  resetLast();
	sheep->create();
	sheep->sensorCalibrate();
  sheep->waitBtnPress();
}

void loop() {
  follow(false); // ok
}

void follow(bool only_follow) {
	s = sheep->sensorReadAll();
	erro = sheep->getState(s, last_erro);
	sheep->drive(VEL_MAX, erro, last_erro);
	last_erro=erro;

  if(s[0] && s[1] && s[2] && !s[3] && s[4] && s[5] && s[6]) {
    inversao = true;
    only_follow = true;
    counterReset();
  }
  if(inversao && !s[0] && !s[1] && !s[2] && !s[3] && !s[4] && !s[5] && !s[6]) {
    pedestre();
    only_follow = false;
    counterReset();
  }
 
  if(!only_follow) {
    counter();
    if(sqrCount[1] > 0) {
      if(s[6] && s[5]) 
        turnRight();   
    } else if(sqrCount[0] > 0) {
      if(s[0] && s[1]) 
        turnLeft();
    }
  }
}

void turnRight() {
	sheep->motorSetVel(0, VEL_MAX);
	do {
		atualizaLast();
		s = sheep->sensorReadAll();
	} while(s_last[5] || !s[5]);
	sqrCount[1]--;
	if(sqrCount[1] > 0) {
    rotatoryRight();
	} else {
    counterReset();
	}
}

void turnLeft() {
	sheep->motorSetVel(VEL_MAX, 0);
	do {
		atualizaLast();
		s = sheep->sensorReadAll();
	} while(s_last[2] || !s[2]);
	sqrCount[0]--;
  if(sqrCount[0] > 0) {
    rotatoryLeft();
  } else {
    counterReset();
  }
}

void rotatoryRight() {
  bool can = true;
  do {
    follow(true);  
    if(s[5] && s[6] && can) {
      sqrCount[1]--;
      can = false;
    } 
    if(!s[5] && !s[6]) {
      can = true;
    }
  } while(sqrCount[1] > 0);
  turnRight();
}

void rotatoryLeft() {
  bool can = true;
  do {
    follow(true);  
    if(s[0] && s[1] && can) {
      sqrCount[0]--;
      can = false;
    } 
    if(!s[0] && !s[1]) {
      can = true;
    }
  } while(sqrCount[0] > 0);
  turnLeft();
}

void pedestre() {
  erro = 0;
  last_erro = 0;
  sheep->motorSetVel(0, 0);
    delay(5000);
    sheep->motorSetVel(VEL_MAX, VEL_MAX);
    do {
      s = sheep->sensorReadAll();
      erro = sheep->getState(s, last_erro);
    } while(erro == last_erro);
}

void counter() {
	if(s[0] && !s_last[0] && !s[1] && s[3]) {
		sqrCount[0]++;
	}
	if(s[6] && !s_last[6] && !s[5] && s[3]) {
		sqrCount[1]++;
	}
	atualizaLast();
 Serial.println(sqrCount[0]);
 Serial.println(sqrCount[1]);
 Serial.println();
}

void atualizaLast() {
  for(int i=0; i<7; i++) {
    s_last[i] = s[i];
  }
}

void resetLast() {
	for(int i=0; i<7; i++) {
    s_last[i] = false;
	}
}

void counterReset() {
  sqrCount[0] = 0;
  sqrCount[1] = 0;
}

