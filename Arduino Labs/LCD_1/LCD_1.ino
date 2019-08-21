#include <LiquidCrystal.h> 

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define sensorPin 0
#define relayPin 5
float Celsius, Fahrenheit, Kelvin;
int sensorValue;
float voltage;
//float sensorValue;

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //sensorValue = analogRead(A0);
  basicPrintDemo();
  //displayOnOffDemo();
  //setCursorDemo();
  //scrollLeftDemo();
  //scrollRightDemo();
  //cursorDemo();
  //createGlyphDemo();
}

void basicPrintDemo(){
  lcd.clear();
  GetTemp();
  //lcd.print("Celsius: ");
  //lcd.println(Celsius);
  //delay(1000);
  lcd.clear();
  lcd.print("Fahrenheit: ");
  lcd.println(Fahrenheit);
  delay(1000);
  lcd.clear();
}

void GetTemp() {
  sensorValue = analogRead(sensorPin);
  voltage = (5)*(4700/(4700+sensorValue));
  Kelvin = (((float(sensorValue) / 1023) * 5) * 100);
  Celsius = Kelvin - 273.15;
  Fahrenheit = (Celsius * 1.8) + 32;
}
