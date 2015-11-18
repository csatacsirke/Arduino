///////////////////////////////////////
//#include <Bridge.h>
//#include <HttpClient.h>
//#include <ArduinoJson.h>
#include "TaskHandler.h"
#include <Servo.h>
#include "PinDef2.h"
#include "robot.h"

Robot robot;

#include "TaskHandler.h"
using namespace TaskHandler;

 #define shutdownTime 10*1000

//Digital pin 7 for reading in the pulse width from the MaxSonar device.
//This variable is a constant because the pin will not change throughout execution of this code.
//const int pwPin = 8; 


/*
const int sonarSensors[] = {8, 9, 3};

//variables needed to store values
long pulse, inches, cm;

int ReadSensor(int pin) {
  pulse = pulseIn(pin, HIGH);
  //147uS per inch
  inches = pulse/147;
  //change inches to centimetres
  return int(inches * 2.54);
  
}*/
/*
void ReadAndPrintSensor(int pin) {
  
    //Used to read in the pulse that is being sent by the MaxSonar device.
  //Pulse Width representation with a scale factor of 147 uS per Inch.

  //pulse = pulseIn(pin, HIGH);
  //147uS per inch
  //inches = pulse/147;
  //change inches to centimetres
  //cm = inches * 2.54;
  
  cm = ReadSensor(pin);
  Serial.print("pin: ");
  Serial.print(pin);
  Serial.print(" ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}*/

//int irany = 90;
/*
void TurnForMillis(int duration) {
  static unsigned long endTime = 0;
  static bool isTurning = false;
  
  unsigned long t = millis();
  if ( isTurning ) {
      if( t >= endTime ) {
      robot.Stop();
      isTurning = false;
    } 
  } else {
    endTime = t + duration;
    Start(0, 30);
    isTurning = true;
  }
  
  

  
}*/


void Task1();
void Task2();

long startupTime;

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  startupTime = millis();
  robot.InitWheels();
  robot.InitSonars();
  srand(millis());

  robot.StartTurret(50);
  Task1();
}



void Task1() {
  robot.Start(30, 0);
  SetTimeout(Task2, 1000);
}
void Task2() {
  robot.Stop();
  SetTimeout(Task1, 1000);
}

void loop() {
  

  if( millis() > startupTime + shutdownTime) {
    robot.StopTurret();
    robot.Stop();
    Serial.write("megallt");
    delay(3000);
    return;
  }
  
  ExecuteNextTask();
  

  
  
  
  /*
  Start(0, 30);
  delay( rand() % 1500 );

  int distance = ReadSensor(FrontSensor);

  if( distance > 50 ) {
    Start(30, 0);
  }

  
  while(true) {
    int dist = ReadSensor(FrontSensor);
    if( dist < 50 ) {
       Stop();
       break;
    }
    delay( 50 );
  }
  
  
  delay(500);
  */
}


