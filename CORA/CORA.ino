#include "lib/Robot.h"

enum state {
	waiting,
	follow,
	turn90,
	pedestrianCrossing
};

#define KP 50
#define VEL_MAX 200
#define BEST_WAY 1 //1 -> direita || 3 -> esquerda
#define TRANSITION 150

Robot *sheep = new Robot();
int btn = 12;
bool button_last = false;
state robot;
int erro;
int last_erro = 0;

void setup() {
	Serial.begin(9600);
	robot = follow;
	pinMode(btn, INPUT);
	sheep->create();
	sheep->sensorCalibrate(btn);
	waitBtnPress();
}

void loop() {
	bool s[5];
	int* sensors;
	static int dir90;
	static bool sOnTrack, sOnTrackAnt;
	static int sqrCount;

	int velRight, velLeft;
  int vel = 200;

	switch(robot) {
		case follow:
			//leitura dos sensores
			sensors = sheep->sensorReadAll();
			for(int i = 0;i < 5;i++) {
				s[i] = sensors[i] >= TRANSITION;
			}
			erro = getState(s);
			//seguindo a linha
      switch(erro) {
      case 2: 
        sheep->stopMotorRight();
        sheep->startMotorLeft(vel);
        break;
      case 1:
        sheep->startMotorRight(vel/2);
        sheep->startMotorLeft(vel);
        break;
      case 0:
        sheep->startMotorRight(vel);
        sheep->startMotorLeft(vel);
        break;
      case -1:
        sheep->startMotorRight(vel);
        sheep->startMotorLeft(vel/2);
        break;
      case -2:
        sheep->startMotorRight(vel);
        sheep->stopMotorLeft();
        break;
      default:
        break;
    }
    delay(90);
      
      Serial.println(erro);
      
			//detecta casos especiais
			if((s[0] && s[4]) && !(s[1] && s[3])) { //dois quadrados indicando que deve escolher direita ou esquerda
				dir90 = BEST_WAY;
				robot = turn90;
			} else if((s[0] || s[4]) && !(s[1] || s[3])) {
				s[0] ? dir90 = 1 : dir90 = 3;
				sqrCount = 1;
				robot = turn90;
			} else if(!s[2] && (s[0] && s[1] && s[3] && s[4])) {
				robot = pedestrianCrossing;
			}
			sheep->motorSetVel(velRight, velLeft);
		break;

		case turn90:
			sOnTrackAnt = s;
			sOnTrack = sheep->sensorRead(dir90) >= 50;
			if(sOnTrack && sOnTrackAnt == 0) robot = follow;
			else {
				sheep->motorSetVel(0,VEL_MAX);
			}
		break;

		case pedestrianCrossing:
			sheep->motorSetVel(VEL_MAX,VEL_MAX);
			if(s[2] == 1 && sheep->sensorRead(dir90) <= 50) {
				delay(5000); //substituir por um contador com uma variável global
			}
		break;
	}
}

int getState(bool s[]) { //retorna posição relativa dos sensores em relação a linha (valores negativos => à direita da linha || valores positivos => à esquerda da linha)
  if(!s[1] && s[2] && !s[3]) {
    return 0;
  }
  if(s[1] && s[2] && !s[3]) {
    return -1;
  }
  if(!s[1] && s[2] && s[3]) {
    return 1;
  }
  if(s[1] && !s[2] && !s[3]) {
    return -2;
  }
  if(!s[1] && !s[2] && s[3]) {
    return 2;
  }
  return 0;
}

void waitBtnPress() {
	int btA, bt = false;
	do {
		btA = bt;
		bt = digitalRead(btn);
	} while(!(bt == 0 && btA == 1));
}
