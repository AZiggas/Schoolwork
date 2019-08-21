#include <Ultrasonic.h>

int relayPin = 9;
int echo = 2;
int trigger = 3;
unsigned long changeTime;
float inchDistance;
bool onFlag = false;

Ultrasonic ultrasonic(trigger, echo);

void setup() {
  Serial.begin(9600);//set Serial Baud rate
  Serial.println("Ultrasonic sensor starting!!!!!");
  
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
}

void loop() {
  float inchDistance;
  long microsec = ultrasonic.timing();
  //Serial.print("microsec: ");
  //Serial.println(microsec);
  inchDistance = ultrasonic.CalcDistance(microsec,Ultrasonic::IN);
  
  if (inchDistance < 20.0) {
    digitalWrite(relayPin, HIGH);
    delay(int(50 * inchDistance));
    digitalWrite(relayPin, LOW);
    delay(1000);
  }
  else{
    digitalWrite(relayPin, LOW);
  }

  Serial.print("inch distance: ");
  Serial.println(inchDistance);
}

