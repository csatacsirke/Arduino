#include <SoftwareSerial.h>

//int bluetoothTx = 17;
//int bluetoothRx = 16;

//SoftwareSerial bluetooth(bluetoothTx, bluetoothRx, true);

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);  // Begin the serial monitor at 9600bps

  Serial.println("Start");

  pinMode(16, OUTPUT);
  pinMode(17, INPUT);
  Serial2.begin(115200);  // The Bluetooth Mate defaults to 115200bps

  
  //Serial2.begin(9600);  
  
  //bluetooth.listen();
  Serial2.print("$");  // Print three times individually
  Serial2.print("$");
  Serial2.print("$");  // Enter command mode

  
  
  delay(100);  // Short delay, wait for the Mate to send back CMD

  //Serial2.print("SU, 9600");

  pinMode(6, OUTPUT);
  
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  
  
  while(Serial2.available()){
      auto asd = Serial2.read();
      Serial.print("x ");
      Serial.print((char)asd);
      //Serial2.listen();
   }
   Serial.println("Setup vege");
   
  Serial2.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  Serial2.begin(9600);  // Start bluetooth serial at 9600
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
   if(Serial2.available()){
      //Serial.print("kaptuk: ");
      digitalWrite(6, HIGH);
      delay(500);
      digitalWrite(6, LOW);
    
      Serial.print((char)Serial2.read());
   }

   if(Serial.available()){
      //Serial.println("elkuldtuk: ");
      char ch = (char)Serial.read();
      Serial.println(/*(char)*/ch);
      Serial2.print(/*(char)*/ch);
   }
}
