#pragma once

#include <Arduino.h>
#include "pindef2.h"
#include <stdlib.h>
#include "DistanceSensor.h"

#define SensorBufferLen 9

class Robot {
  DistanceSensor<FrontSensor> myFrontSensor;  
  DistanceSensor<RightSensor> myRightSensor;  
  DistanceSensor<LeftSensor> myLeftSensor;  

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
    int irany = 90 - direction;
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
    /*int pulse;
    //inches = pulse/147;
    //return int(inches * 2.54);  
    //conversion: cm = pulse * 0.0172789
    for (int i=0;i<SensorBufferLen;++i) {
    //pulse = pulseIn(FrontSensor, HIGH);
    //sensorBuffer_Front[i] = (int)(pulse * 0.0172789f);
    pulse = pulseIn(LeftSensor, HIGH);
    sensorBuffer_Right[i] = (int)(pulse * 0.0172789f);
    pulse = pulseIn(RightSensor, HIGH);
    sensorBuffer_Left[i] = (int)(pulse * 0.0172789f);
    delay(10);
    }*/
    //
    //qsort(sensorBuffer_Front, SensorBufferLen, sizeof(int), compare);
    //qsort(sensorValue_Left, SensorBufferLen, sizeof(int), compare);
    //qsort(sensorValue_Right, SensorBufferLen, sizeof(int), compare);

    //sensorValue_Front = sensorBuffer_Front[(SensorBufferLen/2)];
    //sensorValue_Left = sensorBuffer_Left[(SensorBufferLen/2)];
    //sensorValue_Right = sensorBuffer_Right[(SensorBufferLen/2)];

    myFrontSensor.Update();
    myRightSensor.Update();
    myLeftSensor.Update();
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

private:
  //int sensorValue_Left = 0;
  //int sensorValue_Right = 0;
  //int sensorValue_Front = 0;

  //int sensorBuffer_Left[SensorBufferLen];
  //int sensorBuffer_Right[SensorBufferLen];
  //int sensorBuffer_Front[SensorBufferLen];

  /*
  static int compare(const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
  }
  */

};
