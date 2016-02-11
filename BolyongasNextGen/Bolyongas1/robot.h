#pragma once

#include <Arduino.h>
#include "pindef1.h"
#include <stdlib.h>
#include "DistanceSensor.h"
#include "IrLed.h"
#include "Bluetooth.h"

#define SensorBufferLen 9

#pragma once

class Robot {
  DistanceSensor<FrontSensor> myFrontSensor;  
  DistanceSensor<RightSensor> myRightSensor;  
  DistanceSensor<LeftSensor> myLeftSensor;

  IrLed irLed;

  Bluetooth bluetooth;

  Servo fwservo;
  Servo stservo;
  Servo turret;
  int speed = 0;
  int direction = 0;

  bool ceaseFire = false;
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

  void Fire(int durationMsec) {
    irLed.Fire(durationMsec);
  }

  bool isTargetOnSight() {
    return bluetooth.ReadOnSight();
  }
  bool getTargetAngle() {
    return bluetooth.ReadAngle();
  }

  void UpdateSensors() {

    myFrontSensor.Update();
    myRightSensor.Update();
    myLeftSensor.Update();

    
    bluetooth.Update();
    
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

  
  void setCeaseFire(bool toSet) {
    this->ceaseFire = toSet;
  }
  bool isCeaseFire() {
    return this->ceaseFire;
  }


private:

};
