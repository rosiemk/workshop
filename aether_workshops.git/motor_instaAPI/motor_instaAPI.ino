/*Sketch to increase speed of DC motor based upon data received from Aether platform. 
 * Data is received as number as is trigered by Instagram likes.
 * data received as 1 or 0
 * 
 */

#include <ESP8266WiFi.h>
#include <aether-client.h>

#define motorSpeed D1
#define dir D3

int Speed = 0; // adjustable speed variable

unsigned long previousTime = 0;
unsigned long currentTime;
unsigned long timer = 0;

AetherClient ae;

//const char* ssid = "VM647500-2G";
//const char* password = "kmyhvyay";

const char* ssid = "UAL-IoT";
const char* password = "u4110t2016";

void receiveFunc(int data);
void forwards(long interval);
void increaseSpeed();
void stop();

void setup() {
  Serial.begin(115200);

  pinMode(motorSpeed, OUTPUT);
  pinMode(dir, OUTPUT);

//------------------------------------
  ae.setLogVerbosity(LOG_STANDARD);

  bool connection = false;
  while(!connection)
  {
    connection = ae.connectToAP(ssid, password);
    Serial.print(".");
  }
  ae.connectToServer("Motor", MODE_RECEIVE, DATA_STRING /*CHANGE FROM DATA STRING TO INT*/);
  ae.receiveData(receiveFunc);
//------------------------------------
}

void loop() 
{
  ae.loop();
}

//functions
void receiveFunc(int data){
  Serial.println(data);
  if(data == 1)
  {
    Serial.println("inc");
    increaseSpeed();
    forwards(1000);
    
  }
  else{
    stop();
    }
}

//----------MOVEMENT
void forwards(long interval){
    analogWrite(motorSpeed, Speed); 
    digitalWrite(dir, HIGH);

    currentTime = millis();
    previousTime = millis();

    while(timer <= interval)
    {
      currentTime = millis();
      int delta = currenttime - previousTime;
      timer += delta;
      previousTime = currentTime;
      yield();
    }
}
void increaseSpeed()
{
  Speed += 50;
  if (Speed >= 1023){
    Speed = 0;
  }
}

void stop()
{
  analogWrite(motorSpeed, 0);
}
  
