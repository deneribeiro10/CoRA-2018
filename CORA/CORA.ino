#include "lib/Robot.h"

enum state {
	parado,
	follow,
	turnRight,
  turnLeft,
  preRight,
  preLeft,
  rotatoryRight,
  rotatoryLeft,
	pedestrianCrossing
};

#define VEL_MAX 254
#define BEST_WAY true
#define TURN_TIMER 1200

Robot *sheep = new Robot();
state robot_state;
int erro;
int last_erro = 0;
int sqrCount[2] = {0, 0};
bool *s;
bool s_last[2] = {false, false};

void setup() {
	robot_state = follow;
	sheep->create();
	sheep->sensorCalibrate();
	sheep->waitBtnPress();
}

void loop() {
	switch(robot_state) {
		case follow:
			s = sheep->sensorReadAll();
			erro = sheep->getState(s, last_erro);
			sheep->drive(VEL_MAX, erro, last_erro);
      last_erro=erro;
			sensorPulse(true);
			specialCases();
		break;

		case turnRight:
    robot_state = parado;
   Serial.println("t r");
//      sqrCount[1]--;
//      sheep->motorSetVel(0, VEL_MAX);
//      delay(TURN_TIMER);
//      sheep->motorSetVel(0, 0);
//      if(sqrCount[1] > 0) {
//        robot_state = rotatoryRight;
//      } else {
//        robot_state = follow; // follow
//      }
//      erro = 2;
//      last_erro = 2;
		break;

		case turnLeft:
    robot_state = parado;
    Serial.println("t l");
//      sqrCount[0]--;
//      sheep->motorSetVel(VEL_MAX, 0);
//      delay(TURN_TIMER);
//      sheep->motorSetVel(0, 0);
//      if(sqrCount[0] > 0) {
//        robot_state = rotatoryLeft;
//      } else {
//        robot_state = follow; // follow
//      }
//      erro = -2;
//      last_erro = -2;
		break;

    case preLeft:
    robot_state = parado;
    Serial.println("p l");
//      s = sheep->sensorReadAll();
//      if(s[2] && !s[3]) {
//        sheep->motorSetVel(VEL_MAX, 0);
//      } else if(!s[2] && s[3]) {
//        sheep->motorSetVel(0, VEL_MAX);
//      } else if(s[2] && s[3]) {
//        sheep->motorSetVel(VEL_MAX, VEL_MAX);
//      }
//      sensorPulse(true);
//      specialCases();
    break;

    case preRight:
    robot_state = parado;
    Serial.println("p r");
//      s = sheep->sensorReadAll();
//      if(s[1] && !s[2]) {
//        sheep->motorSetVel(VEL_MAX, 0);
//      } else if(!s[1] && s[2]) {
//        sheep->motorSetVel(0, VEL_MAX);
//      } else if(s[1] && s[2]) {
//        sheep->motorSetVel(VEL_MAX, VEL_MAX);
//      }
//      sensorPulse(true);
//      specialCases();
    break;

    case rotatoryRight:
    robot_state = parado;
    Serial.println("r r");
//      s = sheep->sensorReadAll();
//      if(s[1] && !s[2]) {
//        sheep->stopMotorLeft();
//      } else if(!s[1] && s[2]) {
//        sheep->stopMotorRight();
//      } else {
//        sheep->motorSetVel(VEL_MAX, VEL_MAX);
//      }
//      sensorPulse(false);
//      if(sqrCount[1] == 0) {
//        robot_state = preRight;
//      }
    break;

    case rotatoryLeft:
    robot_state = parado;
    Serial.println("r l");
//      
//      sensorPulse(false);
//      if(sqrCount[0] == 0) {
//        robot_state = preLeft;
//      }
    break;

    case parado: 
      sheep->motorSetVel(0,0);
    break;

		case pedestrianCrossing:
    Serial.println("p c");
			// sheep->motorSetVel(VEL_MAX,VEL_MAX);
			// if(s[2] == 1 && sheep->sensorRead(dir90) <= 50) {
			// 	delay(5000); //substituir por um contador com uma variável global
			// }
			Serial.println("predestrian crossing");
			counterReset();
		break;
	}
}

void specialCases() {
  if(!s[0] && !s[1] && !s[2] && !s[3] && !s[4]) {
    robot_state = parado;
  }
	if(sqrCount[0] == sqrCount[1]) {
    if(s[1] && s[3]) {
      robot_state = BEST_WAY ? turnRight : turnLeft;
    }
	} else if(sqrCount[0] > sqrCount[1] && robot_state != preLeft) {
    robot_state = preLeft;
    return;
	} else if(robot_state != preRight) {
    robot_state = preRight;
    return;
	}

  if(robot_state == preLeft && s[0] && s[1]) {
    robot_state = turnLeft;
  }
  if(robot_state == preRight && s[3] && s[4]) {
    robot_state = turnRight;
  }
	// if((s[0] && s[4]) && !(s[1] && s[3])) { //dois quadrados indicando que deve escolher direita ou esquerda
	// 	dir90 = BEST_WAY;
	// 	robot = turnRight;
	// } else if((s[0] || s[4]) && !(s[1] || s[3])) {
	// 	s[0] ? dir90 = 1 : dir90 = 3;
	// 	sqrCount = 1;
	// 	robot = turnRight;
	// } else if(!s[2] && (s[0] && s[1] && s[3] && s[4])) {
	// 	robot = pedestrianCrossing;
	// }
}

void sensorPulse(bool increase) {
  if(increase) {
    if(!s[0] && s_last[0]) {
      sqrCount[0]++;
    }
    if(!s[4] && s_last[1]) {
      sqrCount[1]++;
    }  
  } else {
    if(!s[0] && s_last[0]) {
      sqrCount[0]--;
    }
    if(!s[4] && s_last[1]) {
      sqrCount[1]--;
    }
  }
  Serial.println(sqrCount[0]);
  Serial.println(sqrCount[1]);
	
	s_last[0] = s[0];
	s_last[1] = s[4];
}

void counterReset() {
	sqrCount[0] = 0;
	sqrCount[1] = 0;
}
