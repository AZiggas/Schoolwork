int ledDelay = 5000;
int redPin = 13;
int yellowPin = 12;
int greenPin = 11;
int redPin2 = 10;
int yellowPin2 = 9;
int greenPin2 = 8;

int flag = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
}

void loop() {

  if(flag == 0){
    digitalWrite(greenPin, HIGH);
    delay(ledDelay);

    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
    delay(ledDelay);
  
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    flag = 1;
  }
  if(flag == 1){
    digitalWrite(greenPin2, HIGH);
    delay(ledDelay);

    digitalWrite(yellowPin2, HIGH);
    digitalWrite(greenPin2, LOW);
    delay(ledDelay);
  
    digitalWrite(redPin2, HIGH);
    digitalWrite(yellowPin2, LOW);
    flag = 0;
  }
}
