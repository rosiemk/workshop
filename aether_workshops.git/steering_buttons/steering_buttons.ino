#include <ESP8266WiFi.h>
#include <aether-client.h>

const int ledR = D2;
const int ledF = D0;
const int ledL = D1;

const int buttonR = D4;
const int buttonF = D5;
const int buttonL = D6;
int switchR = LOW;
int switchF = LOW;
int switchL = LOW;

AetherClient ae;
/*
const char* ssid = "UAL-IoT";
const char* password = "u4110t2016";
*/
//const char* ssid = "401Router";
//const char* password = "WT2A84tx";

//const char* ssid = "VM647500-2G";
//const char* password = "kmyhvyay";

const char* ssid = "UAL-IoT";
const char* password = "u4110t2016";

void setup() 
{
  Serial.begin(115200);
  pinMode(ledF, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledL, OUTPUT);

  pinMode(buttonR, INPUT);
  pinMode(buttonF, INPUT);
  pinMode(buttonL, INPUT);

  ae.setLogVerbosity(LOG_STANDARD);

  bool connection = false;
  while(!connection)
  {
    connection = ae.connectToAP(ssid, password);
    Serial.print(".");
  }
  ae.connectToServer("Buttons", MODE_SEND, DATA_STRING);
  
}

void loop() 
{
  ae.loop();
  
  switchR = digitalRead(buttonR);
  switchF = digitalRead(buttonF);
  switchL = digitalRead(buttonL);
  delay(10);
  //Serial.println(direction_val);

  if(switchR == HIGH && switchF == LOW && switchL == LOW) 
  {
    Serial.println("r");
    digitalWrite(ledR, HIGH);
    ae.sendData("r");
    delay(1000);
   
  }
  else if (switchR == LOW && switchF == HIGH && switchL == LOW)
  {
    Serial.println("f");
    digitalWrite(ledF, HIGH);
    ae.sendData("f");
    delay(1000);
    
  }
  else if (switchR == LOW && switchF == LOW && switchL == HIGH)
  {
    Serial.println("l");
    digitalWrite(ledL, HIGH);
    ae.sendData("l");
    delay(1000);
    
  }
  else {
    digitalWrite(ledF, LOW);
    digitalWrite(ledR, LOW);
    digitalWrite(ledL, LOW);
  }

}

