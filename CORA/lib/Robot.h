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
      Robot();
      Robot(int,int[],int[],int);
      void create();
      void startMotorLeft(int);
      void startMotorRight(int);
      void motorSetVel(int,int);
      void stopMotorLeft();
      void stopMotorRight();
      int* sensorReadAll();
      int sensorRead(int, int);
      void sensorCalibrate(int);
      void drive(int, int);
    private:
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
  }

  Robot::Robot(int enable_left, int motor_left[], int motor_right[], int enable_right) {
    this->enable_left = enable_left;
    this->motor_left[0] = motor_left[0];
    this->motor_left[1] = motor_left[1];
    this->motor_right[0] = motor_right[0];
    this->motor_right[1] = motor_right[1];
    this->enable_right = enable_right;
  }

  void Robot::create() {
    pinMode(this->enable_left, OUTPUT);
    pinMode(this->motor_left[0], OUTPUT);
    pinMode(this->motor_left[1], OUTPUT);
    pinMode(this->motor_right[0], OUTPUT);
    pinMode(this->motor_right[1], OUTPUT);
    pinMode(this->enable_right, OUTPUT);
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

  int* Robot::sensorReadAll() {
    int sen[5];
    for(int i = 0;i < 5;i++) {
      sen[i] = this->sensorRead(this->s[i]);
    }
    return sen;
  }

  void Robot::sensorCalibrate(int btn) {
    bool btA, bt = false;

    //aguarda botão (borda de descida)
    Serial.println("Press button to calibrate maximum value");
    do {
      btA = bt;
      bt = digitalRead(btn);
    } while(!(bt == 0 && btA == 1));

    //calibração máximo
    for(int i = 0;i < 5;i++) {
      sMax[i] = this->sensorRead(this->s[i]);
      Serial.println(sMax[i]);
    }

    //aguarda botão (borda de descida)
    Serial.println("Press button to calibrate minimum value");
    do {
      btA = bt;
      bt = digitalRead(btn);
    } while(!(bt == 0 && btA == 1));

    //calibração mínimo
    for(int i = 0;i < 5;i++) {
      sMin[i] = this->sensorRead(this->s[i]);
      Serial.println(sMin[i]);
    }
    Serial.println("Calibration succesful");
  }

  void Robot::drive(int vel, int turn) {
    switch(turn) {
      case 2: 
        this->stopMotorRight();
        this->startMotorLeft(vel);
        break;
      case 1:
        this->startMotorRight(vel/2);
        this->startMotorLeft(vel);
        break;
      case 0:
        this->startMotorRight(vel);
        this->startMotorLeft(vel);
        break;
      case -1:
        this->startMotorRight(vel);
        this->startMotorLeft(vel/2);
        break;
      case -2:
        this->startMotorRight(vel);
        this->stopMotorLeft();
        break;
      default:
        break;
    }
  }

#endif
