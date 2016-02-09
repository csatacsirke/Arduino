
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "pindef1.h"

class Bluetooth {
  SoftwareSerial bluetooth(bluetoothTx,bluetoothRx);
  bool initialized = false;

  bool onSight = false;
  int angle = 0;
public:
  
  void Update() {
      if( !initialized ) {
        bluetooth.begin(115200);
        bluetooth.listen();
        bluetooth.print("$");
        bluetooth.print("$");
        bluetooth.print("$");
        delay(100);
        bluetooth.println("U,9600,N");
        bluetooth.begin(9600);
        initialized = true;
      }

      if(bluetooth.available()>1){
        bluetooth.write("1");
        //startupTime=startupTime-shutdownTime;
        char robotB = bluetooth.read();
        char angleByte = bluetooth.read();
         /* for(int i=0; i<4; i++){
                angleBytes[i] = bluetooth.read();
            }*/
          if(robotB != 0){
          //látja
            onSight = true;
            angle = angleByte;
            //bluetooth.write("0");
           // startupTime=startupTime-shutdownTime;
          }else{
          //nem látja
            onSight = false;
            //bluetooth.write("1");
          //startupTime=startupTime-shutdownTime;
          }
      }
  bool ReadOnSight() {
    return onSight;
  }
  int ReadAngle() {
    return angle;
  }
};

