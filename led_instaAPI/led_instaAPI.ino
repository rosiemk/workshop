/*Sketch to increase speed of DC motor based upon data received from Aether platform. 
 * Data is received as number as is trigered by Instagram likes.
 * data received as 1 or 0
 * 
 */

#include <ESP8266WiFi.h>
#include <aether-client.h>

#define led1 D3
#define led2 D4
#define led3 D5
#define led4 D6
#define led5 D7

int state = 0;
int prevData = 0;

AetherClient ae;

const char* ssid = "UAL-IoT";
const char* password = "u4110t2016";

void receiveFunc(float _data);

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

//------------------------------------
  ae.setLogVerbosity(LOG_STANDARD);

  bool connection = false;
  while(!connection)
  {
    connection = ae.connectToAP(ssid, password);
    Serial.print(".");
  }
  ae.connectToServer("moonfrogs", MODE_RECEIVE, DATA_NUMBER);
  ae.receiveData(receiveFunc);
//------------------------------------
}

void loop() 
{
  ae.loop();
}

//functions
void receiveFunc(float _data){
  int data = _data;
  if (data != prevData){
    Serial.println(data);
    digitalWrite (led1, HIGH);
    digitalWrite (led2, HIGH);
    digitalWrite (led3, HIGH);
    digitalWrite (led4, HIGH);
    digitalWrite (led5, HIGH);

    state = 1;
    delay(1000);
    Serial.print("x");
   
    
    prevData = data;
  }
  else {
    digitalWrite (led1, LOW);
    digitalWrite (led2, LOW);
    digitalWrite (led3, LOW);
    digitalWrite (led4, LOW);
    digitalWrite (led5, LOW);
    state = 0;
  }

}
