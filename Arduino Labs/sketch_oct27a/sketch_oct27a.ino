#define sensorPin 0
float Celsius, Fahrenheit, Kelvin;
int sensorValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Initializing.....");
}

void loop() {
  // put your main code here, to run repeatedly:
  GetTemp();
  Serial.print("Celsius: ");
  Serial.println(Celsius);
  Serial.print("Fahrenheit: ");
  Serial.println(Fahrenheit);
  Serial.println();

  delay(2000);
}

void GetTemp() {
  sensorValue = analogRead(sensorPin);
  Kelvin = (((float(sensorValue) / 1023) * 5) * 100);
  Celsius = Kelvin - 273.15;
  Fahrenheit = (Celsius * 1.8) + 32;
}

