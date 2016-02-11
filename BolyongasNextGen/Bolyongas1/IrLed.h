#include <Arduino.h>
#include "pindef1.h"
#pragma once
#define onTime 1
#define offTime 17

#define cycleLenMilliSec 100
#define delayBetweenCyclesMilliSec 2000

//used by hunter only (robot#1)

class IrLed {
  bool initialized = false;
public:
  
  void Fire(int durationMsec) {
      if( !initialized ) {
        pinMode(IrLedPin, OUTPUT);  
        initialized = true;
      }
          
      for (int i=0;i<durationMsec*55;i++) {
        digitalWrite(IrLedPin, HIGH);
        delayMicroseconds(onTime);
        digitalWrite(IrLedPin, LOW);
        delayMicroseconds(offTime);
      }
  }
};

