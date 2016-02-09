#pragma once

#include <Arduino.h>
#include "pindef2.h"
#include <stdlib.h>
#include "DistanceSensor.h"
#include "IrSensor.h"
#include "RedLed.h"

#define SensorBufferLen 9

class Robot {
  DistanceSensor<FrontSensor> myFrontSensor;  
  DistanceSensor<RightSensor> myRightSensor;  
  DistanceSensor<LeftSensor> myLeftSensor;

  IrSensor irSensor;

  RedLed redLed;

  Servo fwservo;
  Servo stservo;
  Servo turret;
  int speed = 0;
  int direction = 0;
public:

  void InitSonars() {

    //pinMode(FrontSensor, INPUT);
    //pinMode(RightSensor, INPUT);
    //pinMode(LeftSensor, INPUT);
  }

  void InitWheels() {

    pinMode(TurretPin, OUTPUT);
    pinMode(SteerPin, OUTPUT);
    pinMode(ForwardPin, OUTPUT);

    turret.attach(TurretPin);
    fwservo.attach(ForwardPin);
    stservo.attach(SteerPin);

    fwservo.write(90 + ForwardZero);
    stservo.write(90 + SteerZero);
    turret.write(90);
  }

  // speed: pozitiv: elöre, negativ hatra min: kb -60 max +60
  // direction: 0: egyenesen
  void Start(int speed, int direction) {
    fwservo.write(90 + speed + ForwardZero);
    int irany = 90 + SteerMult*direction;
      //TODO iranyhoz hozzaadni SteerZero-t?
      //TODO irany miert 90 MINUSZ dir?
    stservo.write(irany);  
    this->speed = speed;
    this->direction = direction;
  }
  void Stop() {   
    fwservo.write(90+ForwardZero);
    stservo.write(90+SteerZero);
    this->speed = 0;
    this->direction = 0;
  }

  int GetRotation() {
    return this->direction;
  }
  int GetSpeed() {
    return this->speed;
  }
  void StartTurret(int speed) {
    turret.write(90+speed);
  }
  void StopTurret() {
    turret.write(90);
  }

  void UpdateSensors() {
    myFrontSensor.Update();
    myRightSensor.Update();
    myLeftSensor.Update();

    irSensor.Update();
  }

  int getSensorValue_Front() {
    return myFrontSensor.Read();
  }
  int getSensorValue_Left() {
    return myLeftSensor.Read();
    //return sensorValue_Left;
  }
  int getSensorValue_Right() {
    return myRightSensor.Read();
  }

  bool isSensorPositive() {
    return irSensor.Read();
  }

  void setRedLedOn() {
    redLed.setOn()
  }
  void setRedLedOff() {
    redLed.setOff()
  }

private:

};
