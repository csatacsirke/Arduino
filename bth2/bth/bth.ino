#include <SoftwareSerial.h>

int bluetoothTx = 0;
int bluetoothRx = 1;

const int LED_PIN = 3;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);  // Begin the serial monitor at 9600bps

  Serial.println("Start");

  bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps

  
  //bluetooth.begin(9600);  
  
  bluetooth.listen();
  bluetooth.print("$");  // Print three times individually
  bluetooth.print("$");
  bluetooth.print("$");  // Enter command mode

  
  
  delay(100);  // Short delay, wait for the Mate to send back CMD

  //bluetooth.print("SU, 9600");

  pinMode(LED_PIN, OUTPUT);
  
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  
  
  while(bluetooth.available()){
      auto asd = bluetooth.read();
      Serial.print("x ");
      Serial.print((char)asd);
      //bluetooth.listen();
   }
   Serial.println("Setup vege");
   
  bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  /*
  while(bluetooth.available()){
      Serial.print("x");
      Serial.print((char)bluetooth.read());
   }
   */
  /*
  bluetooth.print("$$$");
  delay(100);
  
*/
//bluetooth.begin(9600);
}

void loop() {
 
  //bluetooth.begin(9600);
  
  // put your main code here, to run repeatedly:
   if(bluetooth.available()){
      //Serial.print("kaptuk: ");
      digitalWrite(LED_PIN, HIGH);
      delay(500);
      digitalWrite(LED_PIN, LOW);
    
      //Serial.print((char)bluetooth.read());
   }
/*
   if(Serial.available()){
      //Serial.println("elkuldtuk: ");
      char ch = (char)Serial.read();
      Serial.println(/*(char)*/ch);
      bluetooth.print(/*(char)*/ch);
   }*/
}
