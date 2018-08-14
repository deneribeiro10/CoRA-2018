#ifndef ROBOT_H
#define ROBOT_H

  #include "Arduino.h"

  class Robot {
    public:
      int enable_left;
      int motor_left[2];
      int motor_right[2];
      int enable_right;
      Robot();
      Robot(int,int[],int[],int);
      void create();
      void startMotorLeft(int);
      void startMotorRight(int);
      void stopMotorLeft();
      void stopMotorRight();
  };

  Robot::Robot() {
    this->enable_left = 11;
    this->motor_left[0] = 10;
    this->motor_left[1] = 6;
    this->motor_right[0] = 5;
    this->motor_right[1] = 3;
    this->enable_right = 2;
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
    //if(vel >= 0) {
      digitalWrite(this->motor_left[0], LOW);
      analogWrite(this->motor_left[1], vel);
    // } else {
    //   digitalWrite(this->motor_left[0], LOW);
    //   analogWrite(this->motor_left[1], vel);
    // }
  }

  void Robot::startMotorRight(int vel) {
    digitalWrite(enable_right, HIGH);
    //if(vel >= 0) {
      digitalWrite(this->motor_right[0], LOW);
      analogWrite(this->motor_right[1], vel);
    // } else {
    //   digitalWrite(this->motor_right[0], LOW);
    //   analogWrite(this->motor_right[1], vel);
    // }
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

#endif
