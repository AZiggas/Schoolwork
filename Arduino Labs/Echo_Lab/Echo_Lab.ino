#include <Ultrasonic.h>

int carRed = 12;
int carYellow = 11;
int carGreen = 10;
int pedRed = 9;
int pedGreen = 8;
int echo = 2;
int trigger = 3;
int crossTime = 5000;
unsigned long changeTime;
float inchDistance;

Ultrasonic ultrasonic(trigger, echo);

void setup() {
  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");
  
  // put your setup code here, to run once:
  pinMode(carRed, OUTPUT);
  pinMode(carYellow, OUTPUT);
  pinMode(carGreen, OUTPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen, OUTPUT);
  digitalWrite(carGreen, HIGH);
  digitalWrite(pedRed, HIGH);
}

void loop() {
  float inchDistance;
  long microsec = ultrasonic.timing();
  Serial.print("microsec: ");
  Serial.println(microsec);

  inchDistance = ultrasonic.CalcDistance(microsec,Ultrasonic::IN);
  
  if (inchDistance < 100.0 && (millis() - changeTime) > 5000) {
    Serial.println("WE DID IT!!!");
    changeLights();
  }

  Serial.print("inch distance: ");
  Serial.println(inchDistance);
  
  delay(1000);
}

void changeLights() {
  digitalWrite(carGreen, LOW);
  digitalWrite(carYellow, HIGH);
  delay(2000);

  digitalWrite(carYellow, LOW);
  digitalWrite(carRed, HIGH);
  delay(1000);

  digitalWrite(pedRed, LOW);
  digitalWrite(pedGreen, HIGH);
  delay(crossTime);

  for (int x=0; x<10; x++) {
    digitalWrite(pedGreen, HIGH);
    delay(250);
    digitalWrite(pedGreen, LOW);
    delay(250);
  }

  digitalWrite(pedRed, HIGH);
  delay(500);

  digitalWrite(carGreen, HIGH);
  digitalWrite(carRed, LOW);
  delay(1000);

  changeTime = millis();
}

