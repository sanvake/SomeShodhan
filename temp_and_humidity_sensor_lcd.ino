#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>
#define RS 12
#define EN 11
#define D4 5
#define D5 4
#define D6 3
#define D7 2
#define dataPin A1 //DHT connected to 2 
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(dataPin, DHT11);
int msensor=A5;
void setup() {
  
  dht.begin();
  lcd.begin(20, 4);
  lcd.clear();
  pinMode(msensor,INPUT);
  lcd.setCursor(0, 0);
  lcd.print("Data Recieved");
  delay(2000);
}
void loop() {
  float t = dht.readTemperature(); 
  float h = dht.readHumidity(); 
  float m = analogRead(msensor);
  m= map(m, 1023,300,0,100);
  lcd.setCursor(0, 1);
  lcd.print("Temperature:");
  lcd.print(t);
  lcd.print(" C");
  lcd.setCursor(0, 2);
  lcd.print("Humidity:");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 3);
  lcd.print("Soil:");
  lcd.print(m);
  lcd.print(" %");
 
  delay(2000);
}
