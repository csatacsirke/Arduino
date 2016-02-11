#include <Arduino.h>
#include "pindef2.h"

//used by hunter only (robot#1)

class RedLed {
  bool initialized = false;
public:

  void SetOn() {
    if (!initialized) {
      pinMode(RedLedPin, OUTPUT);
      initialized = true;
    }
    digitalWrite(RedLedPin, HIGH);
  }
  void SetOff() {
    if (!initialized) {
      pinMode(RedLedPin, OUTPUT);
      initialized = true;
    }
    digitalWrite(RedLedPin, LOW);
    
  }
  
};

