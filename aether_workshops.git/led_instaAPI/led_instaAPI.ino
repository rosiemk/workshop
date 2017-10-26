/*Sketch to increase speed of DC motor based upon data received from Aether platform. 
 * Data is received as number as is trigered by Instagram likes.
 * data received as 1 or 0
 * 
 */

#include <ESP8266WiFi.h>
#include <aether-client.h>

#define led D2

int state = 0;
int prevData = 0;

AetherClient ae;

const char* ssid = "UAL-IoT";
const char* password = "u4110t2016";

void receiveFunc(float _data);

void setup() {
  Serial.begin(115200);

  pinMode(led, OUTPUT);

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
    digitalWrite (led, HIGH);
    state = 1;
    Serial.println(state);
    delay(500);
    
    data = prevData;
  }
  else {
    digitalWrite (led, LOW);
    state = 0;
    Serial.println(state);
  }

}
