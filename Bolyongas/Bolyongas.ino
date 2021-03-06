///////////////////////////////////////
//#include <Bridge.h>
//#include <HttpClient.h>
//#include <ArduinoJson.h>
#include "TaskHandler.h"
#include <Servo.h>
#include "robot.h"

Robot robot;

#include "TaskHandler.h"
using namespace TaskHandler;

 #define shutdownTime 60*1000

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


void EloreTask();
void SensorTask();
/*void KorrekcioBalraTask();
void KorrekcioJobbraTask();
*/
long startupTime;


void SensorTask() {
  //Serial.println("Sensor_task");
  robot.UpdateSensors();
  SetTimeout(SensorTask, 20);
  //Serial.println("task...");
}

void ForgasTask();



void EloreTask() {
  Serial.println("Elore_task");
  int dist_front = robot.getSensorValue_Front();
  int dist_left = robot.getSensorValue_Left();
  int dist_right = robot.getSensorValue_Right();
  int sp = robot.GetSpeed();
  int rot = robot.GetRotation();
  /*
  Serial.print("dists: ");
  Serial.print(dist_front);
  Serial.print(", l: ");
  Serial.print(dist_left);
  Serial.print(", r: ");
  Serial.println(dist_right);
  */
  /*
  Serial.print(", sp: ");
  Serial.print(sp);
  Serial.print(", rot: ");
  Serial.println(rot);
*/
  if( dist_front < 60 ) {
    //fal elotte
    Serial.print("    dist small: ");
    Serial.println(dist_front);
    if (sp > 0) {robot.Stop();}
    SetTimeout(ForgasStartTask, 0);  
  } else if (sp > 0 && dist_left < 30 && dist_right > 60) {
    //fal balra
    Serial.println("    correction_right");
    robot.Start(sp, 15);
    SetTimeout(EloreTask, 20); 
  } else if (sp > 0 && dist_right < 30 && dist_left > 60) {
    //fal jobbra
    Serial.println("    correction_left");
    robot.Start(sp, -15);
    SetTimeout(EloreTask, 20); 
  } else {
    //nincs akadaly
    robot.Start(30, 0);
    SetTimeout(EloreTask, 20);  
  }
}

void ForgasEndTask() {
  Serial.println("ForgasEnd_task");
  robot.Stop();
  SetTimeout(EloreTask, 0);
}

void ForgasStartTask() {
  Serial.println("ForgasStart_task");
  int dist_left = robot.getSensorValue_Left();
  int dist_right = robot.getSensorValue_Right();
  if (dist_left > 100) {
    Serial.println("    to_left");
    robot.Start(0, -20 );
    SetTimeout(ForgasEndTask, 2000);
  } else if (dist_right > 100) {
    Serial.println("    to_right");
    robot.Start(0, 20 );
    SetTimeout(ForgasEndTask, 2000);
  } else {
    Serial.println("    random");
    robot.Start(0, (rand()%2 == 0 ? -1 : 1) * 20 );
    SetTimeout(ForgasEndTask, 2000);
  }

  
}

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  startupTime = millis();
  robot.InitWheels();
  robot.InitSonars();
  srand(millis());

  //robot.StartTurret(50);
  EloreTask();
  SensorTask();
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
}


