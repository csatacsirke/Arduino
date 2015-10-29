///////////////////////////////////////
//#include <Bridge.h>
//#include <HttpClient.h>
//#include <ArduinoJson.h>
#include <Servo.h>

//HttpClient client;
Servo fwservo;
Servo stservo; 

//Digital pin 7 for reading in the pulse width from the MaxSonar device.
//This variable is a constant because the pin will not change throughout execution of this code.
//const int pwPin = 8; 
const int sonarSensors[] = {8, 9, 10};
#define FrontSensor 9
#define LeftSensor 10
#define RightSensor 8


//variables needed to store values
long pulse, inches, cm;


void InitWheels() {

  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  fwservo.attach(11);
  stservo.attach(10);
  
  fwservo.write(90);
  stservo.write(90);
  
}

void InitSonars() {
  
  pinMode(sonarSensors[0], INPUT);
  pinMode(sonarSensors[1], INPUT);
  pinMode(sonarSensors[2], INPUT);
}

int ReadSensor(int pin) {
  pulse = pulseIn(pin, HIGH);
  //147uS per inch
  inches = pulse/147;
  //change inches to centimetres
  return int(inches * 2.54);
  
}

void ReadAndPrintSensor(int pin) {
  
    //Used to read in the pulse that is being sent by the MaxSonar device.
  //Pulse Width representation with a scale factor of 147 uS per Inch.
/*
  pulse = pulseIn(pin, HIGH);
  //147uS per inch
  inches = pulse/147;
  //change inches to centimetres
  cm = inches * 2.54;
  */
  cm = ReadSensor(pin);
  Serial.print("pin: ");
  Serial.print(pin);
  Serial.print(" ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
}

int irany = 90;

void Stop() {
  fwservo.write(90);
  stservo.write(90);
}

// speed: pozitiv: elöre, negativ hatra min: kb -60 max +60
// direction: 0: egyenesen
void Start(int speed, int direction) {

  //const int sebesseg = 0;
  fwservo.write(90 + speed);

  //unsigned long t = millis();

  //int irany = 90 + int(sebesseg * sin( double(t) / 1000 / 5 * 2 * 3.14 ));
  int irany = 90 - direction;
  stservo.write(irany);  
}

void TurnForMillis(int duration) {
  static unsigned long endTime = 0;
  static bool isTurning = false;
  
  unsigned long t = millis();
  if ( isTurning ) {
      if( t >= endTime ) {
      Stop();
      isTurning = false;
    } 
  } else {
    endTime = t + duration;
    Start(0, 30);
    isTurning = true;
  }
  
  

  
}

void setup() {

  //This opens up a serial connection to shoot the results back to the PC console
  Serial.begin(9600);

  InitWheels();
  InitSonars();
  srand(millis());
}


void loop() {
  
  //ReadAndPrintSensor(sonarSensors[0]);
  //ReadAndPrintSensor(sonarSensors[1]);
  //ReadAndPrintSensor(sonarSensors[2]);

  static bool isMoving = false;
  
  TurnForMillis(1500);

  if( isMoving ) {
    
  }
  int distance = ReadSensor(FrontSensor);

  if(distance > 100 ) {
  
  }
  
  
  
  delay(500);
}

