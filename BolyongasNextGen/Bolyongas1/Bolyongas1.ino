///////////////////////////////////////
//#include <Bridge.h>
//#include <HttpClient.h>
//#include <ArduinoJson.h>
#include "TaskHandler.h"
#include <Servo.h>
#include "robot.h"

Robot robot;

int shotsFired = 0;

 #define shutdownTime 300*1000

void EloreTask();
void SensorTask();

long startupTime;


void SensorTask() {
  //Serial.println("Sensor_task");
  robot.UpdateSensors();
  TaskHandler::SetTimeout(SensorTask, 20);
  //Serial.println("task...");
}

void ForgasStartTask();

void EndAimTurnTask() {
  Serial.println("EndAimTurn_task");
  robot.Stop();
  robot.Fire(100);
  shotsFired += 1;
  TaskHandler::SetTimeout(AimTask, 25);
}

void EndCeaseFireTask() {
  robot.setCeaseFire(false);
}

void AimTask() {
  Serial.println("Aim_task");
  if (shotsFired >= 5) {
    robot.setCeaseFire(true);
    shotsFired = 0;
    TaskHandler::SetTimeout(EndCeaseFireTask, 20000);
  }
  if (robot.isCeaseFire() || !robot.isTargetOnSight()) {
    TaskHandler::SetTimeout(EloreTask, 0);
    return;
  }
  int angle = robot.getTargetAngle();
  int sgn = angle < 0 ? -1 : 1;
  int absAngle = angle < 0 ? -angle : angle;
  robot.Start(0,sgn*30);
  int turnDurationMsec;
  if (absAngle < 3) {
    turnDurationMsec = 0;
  } else if (sgn < 0) {
    turnDurationMsec = 20*absAngle; //balra
  } else {
    turnDurationMsec = 16*absAngle; //jobbra
  }
  TaskHandler::SetTimeout(EndAimTurnTask, turnDurationMsec);
  //extra fire, during turn
  if (turnDurationMsec >= 2) {
    robot.Fire(turnDurationMsec/2);
  }
}

void EloreTask() {
  Serial.println("Elore_task");

  //stop if shot
  if (!robot.isCeaseFire() && robot.isTargetOnSight()) {
    robot.Stop();
    
    TaskHandler::SetTimeout(AimTask, 0);  
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
    robot.Start(25, 0);
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
  SensorTask();
}

void loop() {

 // DEBUG
 /*
  robot.Start(0, -30 );
  delay(500);
  robot.Stop();
  delay(3000);
  */
  
  if( millis() > startupTime + shutdownTime) {
    robot.Stop();
    Serial.write("megallt");
    delay(3000);
    return;
  }
  
  TaskHandler::ExecuteNextTask();
}


