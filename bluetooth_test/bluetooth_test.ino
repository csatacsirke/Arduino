#include <SoftwareSerial.h>

int bluetoothTx = 15;
int bluetoothRx = 14;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  bluetooth.begin(115200);
  bluetooth.print("$$$");
  delay(100);
  bluetooth.println("U,9600,N");

  bluetooth.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   if(bluetooth.available()){
      Serial.print("x");
      Serial.print((char)bluetooth.read());
   }

   if(Serial.available()){
      Serial.println("y");
     bluetooth.print((char)Serial.read());
   }
}
