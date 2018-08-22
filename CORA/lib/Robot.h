#ifndef ROBOT_H
#define ROBOT_H

  #include "Arduino.h"

  class Robot {
    public:
      int enable_left;
      int motor_left[2];
      int motor_right[2];
      int enable_right;
      int s[5];
      int btn;
      Robot();
      Robot(int,int[],int[],int, int);
      void create();
      void startMotorLeft(int);
      void startMotorRight(int);
      void motorSetVel(int,int);
      void stopMotorLeft();
      void stopMotorRight();
      bool* sensorReadAll();
      int sensorRead(int, int);
      void sensorCalibrate();
      void drive(int, int, int);
      void waitBtnPress();
      int getState(bool*, int);
      void sensorPrint();
      void sensorReset();
      int sMax[5];
      int sMin[5];
  };

  Robot::Robot() {
    this->enable_left = 11;
    this->motor_left[0] = 10;
    this->motor_left[1] = 6;
    this->motor_right[0] = 5;
    this->motor_right[1] = 3;
    this->enable_right = 2;
    for(int i = 0;i < 5;i++) {
      this->s[i] = i;
    }
    this->btn = 12;
  }

  Robot::Robot(int enable_left, int motor_left[], int motor_right[], int enable_right, int btn) {
    this->enable_left = enable_left;
    this->motor_left[0] = motor_left[0];
    this->motor_left[1] = motor_left[1];
    this->motor_right[0] = motor_right[0];
    this->motor_right[1] = motor_right[1];
    this->enable_right = enable_right;
    this->btn = btn;
  }

  void Robot::create() {
    Serial.begin(9600);
    pinMode(this->enable_left, OUTPUT);
    pinMode(this->motor_left[0], OUTPUT);
    pinMode(this->motor_left[1], OUTPUT);
    pinMode(this->motor_right[0], OUTPUT);
    pinMode(this->motor_right[1], OUTPUT);
    pinMode(this->enable_right, OUTPUT);
    pinMode(this->btn, INPUT);
  }

  void Robot::startMotorLeft(int vel) {
    digitalWrite(enable_left, HIGH);
    digitalWrite(this->motor_left[0], LOW);
    analogWrite(this->motor_left[1], vel);
  }

  void Robot::startMotorRight(int vel) {
    digitalWrite(enable_right, HIGH);
    digitalWrite(this->motor_right[0], LOW);
    analogWrite(this->motor_right[1], vel);
  }

  void Robot::motorSetVel(int velR, int velL) {
    if(velR > 0)
      this->startMotorRight(velR);
    else
      this->stopMotorRight();
    if(velL > 0)
      this->startMotorLeft(velL);
    else
      this->stopMotorLeft();
  }

  void Robot::stopMotorLeft() {
    digitalWrite(this->enable_left, LOW);
    digitalWrite(this->motor_left[0], LOW);
    digitalWrite(this->motor_left[1], LOW);
  }

  void Robot::stopMotorRight() {
    digitalWrite(this->enable_right, LOW);
    digitalWrite(this->motor_right[0], LOW);
    digitalWrite(this->motor_right[1], LOW);
  }

  int Robot::sensorRead(int sen, int amostras = 32) {
      int media = 0;
      for(int j = 0;j < amostras;j++) {
        media += analogRead(this->s[sen]);
      }
      return media/amostras;
  }

  bool* Robot::sensorReadAll() {
    bool sen[5];
    for(int i=0; i<5; i++) {
      sen[i] = map(this->sensorRead(i), this->sMin[i], this->sMax[i], 0, 100) < 50;
    }
    return sen;
  }

  void Robot::drive(int vel, int turn = 0, int last_erro = 0) {
      const int Kp = 90;
      const int Kd = 60;
      const int baseSpeed = 200;

      int motorSpeed = Kp * turn + Kd * (turn - last_erro);
      int rightMotorSpeed = baseSpeed - motorSpeed;
      int leftMotorSpeed = baseSpeed + motorSpeed;

      if (rightMotorSpeed > vel ) rightMotorSpeed = vel; // prevent the motor from going beyond max speed
      if (leftMotorSpeed > vel ) leftMotorSpeed = vel; // prevent the motor from going    beyond max speed
      if (rightMotorSpeed < 0) rightMotorSpeed = 0; // keep the motor speed positive
      if (leftMotorSpeed < 0) leftMotorSpeed = 0;
      // keep the motor speed positive
      this->motorSetVel(rightMotorSpeed, leftMotorSpeed);
  }

  void Robot::sensorCalibrate() {
    bool btA, bt = false;

    Serial.println("Press button to calibrate");
    this->sensorReset();
    this->waitBtnPress();
    for(int i = 0; i < 5;i++) {
        this->sMax[i] = max(this->sensorRead(i), this->sMax[i]);
    }
    this->waitBtnPress();
    for(int i = 0; i < 5;i++) {
      this->sMin[i] = min(this->sensorRead(i), this->sMin[i]);
    }
    Serial.println("Calibration ok");
  }

  void Robot::waitBtnPress() {
  	bool btA, bt = false;
  	do {
  		btA = bt;
  		bt = digitalRead(this->btn);
  	} while(bt || !btA);
  }

  int Robot::getState(bool* s, int last) {
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
    if(s[1] && s[2] && s[3]) {
        return 0;
    }
    return last;
  }

  void Robot::sensorPrint() {
    for(int i=0; i<5; i++) {
      Serial.println( this->sensorRead(i) );
    }
    Serial.println();
  }

  void Robot::sensorReset() {
    for(int i=0; i<5; i++) {
      this->sMin[i] = 1023;
      this->sMax[i] = 0;
    }
  }

#endif
