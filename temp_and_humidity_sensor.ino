#include <DHT.h>
#include <DHT_U.h>

#define dataPin 2 //DHT connected to 2 
DHT dht(dataPin, DHT11);
int msensor=A0;
void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("CLEARDATA");        //This string is defined as a 
                                      // commmand for the Excel VBA 
                                      // to clear all the rows and columns
  Serial.println("LABEL,Computer Time,Temperature,Humidity, Soil Moisture");  
                                      //LABEL command creates label for 
                                      // columns in the first row with bold font
}
void loop() {
  digitalWrite(LED_BUILTIN,HIGH);
  float t = dht.readTemperature(); 
  float h = dht.readHumidity(); 
  float m = analogRead(msensor);
  m= map(m, 1023,300,0,100);
  Serial.print("DATA,TIME,");
  Serial.print(t);
  Serial.print(" C ,");
  Serial.print(h);
  Serial.print(" % ,");
  Serial.print(m);
  Serial.println(" % ");
  Serial.println("SAVEWORKBOOK");
  delay(2000);
}
