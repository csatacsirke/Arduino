#include "MedianQueue.h"
#include <Arduino.h>

template<int pin>
class DistanceSensor {
  MedianQueue<9> buffer;
  bool initialized = false;
public:
  
  void Update() {
      if( !initialized ) {
        pinMode(pin, INPUT);
        initialized = true;
      }
      
      float pulse = pulseIn(pin, HIGH);
      int cm = (int)(pulse * 0.0172789f * 2.54f);
      Serial.print("sensor: ");
      Serial.println(cm);
      buffer.Push(cm);
  }
  int Read() {
    buffer.GetMedian();
  }
};

