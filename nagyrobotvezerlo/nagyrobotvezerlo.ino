#include <Bridge.h>
#include <HttpClient.h>
//#include <ArduinoJson.h>
#include <Servo.h>

HttpClient client;
Servo fwservo, stservo, penservo;

#define UP 110
#define DOWN 20

int status = 0;
int x_value = 2000;
int y_value = 2000;
int z_value = 1000;


int irany = 90;

void setup() {
//  Serial.begin(9600);
//  while (!Serial);
//  Serial.println("Starting bridge...");
  pinMode(13, OUTPUT);
  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  fwservo.attach(11);
  stservo.attach(10);
  //penservo.attach(9);
  fwservo.write(90);
  stservo.write(90);
  //penservo.write(UP);
  //Bridge.begin();
}

void loop() {
  /*client.get("http://dweet.io/get/latest/dweet/for/eitdigitalbapg");
  String controlJson = client.readString();
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(controlJson);
  if (!root.success()) {
 //   Serial.println("parseObject() failed");
    return;
  }
  status = root["with"][0]["content"]["status"];
  x_value = constrain(root["with"][0]["content"]["x"], -1000, 1000);
  y_value = constrain(root["with"][0]["content"]["y"], -500, 1000);
  z_value = constrain(root["with"][0]["content"]["z"], 1500, 3000);*/
 // Serial.print(">");
 // Serial.print(status);
 // Serial.print("<");
 // Serial.println();

  //digitalWrite(13, HIGH);

  /*
  const int sebesseg = 20;
  fwservo.write(90 + sebesseg);

  unsigned long t = millis();

  int irany = 90 + int(sebesseg * sin( double(t) / 1000 / 5 * 2 * 3.14 ));
  
  stservo.write(irany);
*/
  const int sebesseg = 0;
  fwservo.write(90 + sebesseg);

  //unsigned long t = millis();

  //int irany = 90 + int(sebesseg * sin( double(t) / 1000 / 5 * 2 * 3.14 ));
  int irany = 90 - 30;
  stservo.write(irany);
  
  //fwservo.write(100);
  //stservo.write(90);
 /*
  if (true) {
    digitalWrite(13, HIGH);
    fwservo.write(map(y_value,-500,1000,140,40));
    stservo.write(map(x_value,-1000,1000,140,40));
    if (z_value <2500) {
      penservo.write(DOWN);
    } else {
      penservo.write(UP);
    }
  } else { // stop servos
    digitalWrite(13, LOW);
    fwservo.write(90);
    stservo.write(90);
    penservo.write(UP);
  }
  */
  //Serial.println(status);
}

