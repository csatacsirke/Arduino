#include "MedianQueue.h"
#include <Arduino.h>

template<int pin>
class DistanceSensor {
  MedianQueue<7> buffer;
  bool initialized = false;
public:
  
  void Update() {
      if( !initialized ) {
        pinMode(pin, INPUT);
        initialized = true;
      }

      //static int asd = 0;
      
      float pulse = pulseIn(pin, HIGH);
      int cm = (int)(pulse * 0.0172789f);
      /*if (asd % 10 == 0) {
        Serial.print("sensor: ");
        Serial.print(cm);
        Serial.print(", pin: ");
        Serial.println(pin);
      }*/
      //asd++;
      buffer.Push(cm);
  }
  int Read() {
    return buffer.GetMedian();
  }
};

