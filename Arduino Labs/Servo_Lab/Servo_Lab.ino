#include <Servo.h>

Servo servo1;

void setup() {
  // put your setup code here, to run once:
  servo1.attach(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  int angle = analogRead(0);
  angle = map(angle, 0, 1023, 0, 180);
  servo1.write(angle);
  delay(15);
}
