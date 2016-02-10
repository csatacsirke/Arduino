///////////////////////////////////////
//#include <Bridge.h>
//#include <HttpClient.h>
//#include <ArduinoJson.h>
#include "TaskHandler.h"
#include <Servo.h>
#include "robot.h"

Robot robot;

#include "TaskHandler.h"

 #define shutdownTime 300*1000

void EloreTask();
void UpdateSensorsTask();

long startupTime;

void EndInvulnerabilityTask() {
  robot.setInvulnerable(false);
}

void UpdateSensorsTask() {
  //Serial.println("Sensor_task");
  robot.UpdateSensors();
  TaskHandler::SetTimeout(UpdateSensorsTask, 20);
  //Serial.println("task...");
}

void ForgasStartTask();

void RestartAfterHitTask() {
  Serial.println("RestartAfterHit_task");
  robot.setRedLedOff();
  robot.setInvulnerable(true);
  TaskHandler::SetTimeout(EloreTask, 0);
  TaskHandler::SetTimeout(EndInvulnerabilityTask, 10000);
}

void EloreTask() {
  Serial.println("Elore_task");

  //stop if shot
  if (!robot.isInvulnerable() && robot.isSensorPositive()) {
    robot.Stop();
    robot.setRedLedOn();
    TaskHandler::SetTimeout(RestartAfterHitTask, 10000);  
    return;
  }
  
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
  Serial.println(dist_right);*/
  
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
    TaskHandler::SetTimeout(ForgasStartTask, 0);  
  } else if (sp > 0 && dist_left < 30 && dist_right > 60) {
    //fal balra
    Serial.println("    correction_right");
    robot.Start(sp, 15);
    TaskHandler::SetTimeout(EloreTask, 20); 
  } else if (sp > 0 && dist_right < 30 && dist_left > 60) {
    //fal jobbra
    Serial.println("    correction_left");
    robot.Start(sp, -15);
    TaskHandler::SetTimeout(EloreTask, 20); 
  } else {
    //nincs akadaly
    robot.Start(30, 0);
    TaskHandler::SetTimeout(EloreTask, 20); 
  }
}

void ForgasEndTask() {
  Serial.println("ForgasEnd_task");
  robot.Stop();
  TaskHandler::SetTimeout(EloreTask, 0);
}

void ForgasStartTask() {
  Serial.println("ForgasStart_task");
  int dist_left = robot.getSensorValue_Left();
  int dist_right = robot.getSensorValue_Right();
  if (dist_left > 100) {
    Serial.println("    to_left");
    robot.Start(0, -20 );
    TaskHandler::SetTimeout(ForgasEndTask, 2000);
  } else if (dist_right > 100) {
    Serial.println("    to_right");
    robot.Start(0, 20 );
    TaskHandler::SetTimeout(ForgasEndTask, 2000);
  } else {
    Serial.println("    random");
    robot.Start(0, (rand()%2 == 0 ? -1 : 1) * 20 );
    TaskHandler::SetTimeout(ForgasEndTask, 2000);
  }

  
}

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);
  startupTime = millis();
  robot.InitWheels();
  robot.InitSonars();
  srand(millis());

  EloreTask();
  UpdateSensorsTask();
}

void loop() {
  
  if( millis() > startupTime + shutdownTime) {
    robot.Stop();
    Serial.write("megallt");
    delay(3000);
    return;
  }
  
  TaskHandler::ExecuteNextTask();
}


