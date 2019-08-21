#define sensorPin 0
#define relayPin 5
float Celsius, Fahrenheit, Kelvin;
int sensorValue;
float tempHigh = 70.00;
float tempLow = 78.00;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initializing.....");
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  GetTemp();
  Serial.print("Celsius: ");
  Serial.println(Celsius);
  Serial.print("Fahrenheit: ");
  Serial.println(Fahrenheit);
  Serial.println();
  if(Fahrenheit >= tempHigh){
    digitalWrite(relayPin, LOW);
  }
  else if(Fahrenheit <= tempLow){
    digitalWrite(relayPin, HIGH);
  }

  delay(2000);
}

void GetTemp() {
  sensorValue = analogRead(sensorPin);
  Kelvin = (((float(sensorValue) / 1023) * 5) * 100);
  Celsius = Kelvin - 273.15;
  Fahrenheit = (Celsius * 1.8) + 32;
}

