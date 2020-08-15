#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

#ifndef STASSID
#define STASSID "" //SSiD
#define STAPSK  "" //Password
#endif

#define dataPin 0          //pin where the dht11 is connected
#define msensor A0
char* ssid = STASSID;
char* password = STAPSK;

String api_key = "";     //  Enter your Write API key from ThingSpeak
const char* server = "api.thingspeak.com";

DHT dht(dataPin, DHT11);



long utcOffsetInSeconds = 5*60*60+60*30; // +5.5 UTC
int h,m,s;
float hm,t,sm;
int flag=0;
unsigned long previousMillis = 0; 
const long interval = 1000; 



WiFiClient client;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
Serial.begin(9600);
dht.begin();
lcd.init();
lcd.backlight();
WiFi.begin(ssid, password);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Connecting Wifi");
lcd.setCursor(0, 1);

while ( WiFi.status() != WL_CONNECTED ) {
delay ( 500 );
lcd.print(".");
Serial.print ( "." );
}
timeClient.begin();
lcd.clear();
}

void loop() {

unsigned long currentMillis = millis();
if (currentMillis - previousMillis >= interval) {
previousMillis = currentMillis;
hm = dht.readHumidity();
t = dht.readTemperature();
sm = analogRead(msensor);
sm= map(sm, 550,0,0,100);
timeClient.forceUpdate();
h = timeClient.getHours();
m = timeClient.getMinutes();
s = timeClient.getSeconds();

Serial.print(timeClient.getHours());
Serial.print(":");
Serial.print(timeClient.getMinutes());
Serial.print(":");
Serial.println(timeClient.getSeconds());

//Serial.println(timeClient.getFormattedTime());
if((m==15||m==30||m==45||m==0)&&(s==0||s=1||s==2))// checkes and uploades every 15 mninutes
{ 
  if(flag==0)
  {
    if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
    {  
          lcd.setCursor(0, 0);
          lcd.print("uploading");   
          String data_to_send = api_key;
          data_to_send += "&field1=";
          data_to_send += hm;
          data_to_send += "&field2=";
          data_to_send += t;
          data_to_send += "&field3=";
          data_to_send += sm;
          data_to_send += "\r\n\r\n";
          client.print("POST /update HTTP/1.1\n");
          client.print("Host: api.thingspeak.com\n");
          client.print("Connection: close\n");
          client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(data_to_send.length());
          client.print("\n\n");
          client.print(data_to_send);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("uploaded");
     }
     flag=1;
  }  
   client.stop();  
}     
if((m==17||m==32||m==47||m==2)&&(s==0||s==1||s==2))
{
  lcd.clear();
  flag=0;
}

lcd.setCursor(12, 0);
if (h <= 9){
lcd.print("0");
lcd.print(h);}
else{
lcd.print(h);}
lcd.print(":");
if (m <= 9){
lcd.print("0");
lcd.print(m);}
else{
lcd.print(m);}
lcd.print(":");
if (s <= 9){
lcd.print("0");
lcd.print(s);}
else{
lcd.print(s);}

lcd.setCursor(0, 1);
lcd.print("Temperature: ");
lcd.setCursor(13, 1);
lcd.print(t);
lcd.print("C");
lcd.setCursor(0, 2);
lcd.print("Humidity: ");
lcd.setCursor(13, 2);
lcd.print(hm);
lcd.print("%");
lcd.setCursor(0, 3);
lcd.print("Moisture: ");
lcd.setCursor(13, 3);
lcd.print(sm);
lcd.print("%");
}
}
