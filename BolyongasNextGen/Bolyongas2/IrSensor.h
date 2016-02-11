#include <Arduino.h>
#include "pindef2.h"

//used by prey only (robot#2)

class IrSensor {
  int sensorPositiveCount = 0;
  bool initialized = false;
public:
  
  void Update() {
      if( !initialized ) {
        pinMode(IrSensorPin1, INPUT);
        pinMode(IrSensorPin2, INPUT);
        pinMode(IrSensorPin3, INPUT);
        pinMode(IrSensorPin4, INPUT);
        pinMode(IrSensorPinOn, OUTPUT);
        initialized = true;
      }
      
        digitalWrite(IrSensorPinOn, HIGH); //sensor enable
        if (digitalRead(IrSensorPin1) == 0 || digitalRead(IrSensorPin2) == 0 || digitalRead(IrSensorPin3) == 0 || digitalRead(IrSensorPin4) == 0) {
          sensorPositiveCount++;
        } else {
          sensorPositiveCount = 0;
        }
  }
  bool Read() {
    return sensorPositiveCount >= 2;
  }
};

