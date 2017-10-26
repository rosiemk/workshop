/*  
Sketch for remotely controlled robot.  
Robot is 2 x DC motors and H-bridge L293D motor driver 
Controlled by interactive circuits connected throguh aether platfrom.
Test sketch for controlling sketch is steering_buttons.
*/

#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <Hash.h>

//set pins for right and left motor.

//RIGHT motor A
#define speedR D1 //pwm  GPIO5
#define dirR D3 // GPIO3

// LEFT motor B
#define speedL D2 //pwm GPIO4
#define dirL D4 // GPIO2

int Speed = 1023;

//bool moving = false;
unsigned long previousTime = 0; 
unsigned long currentTime;
unsigned long timer = 0;

ESP8266WiFiMulti wiFiMulti;

WebSocketsServer webSocket = WebSocketsServer(8080);

//const char* ssid = "401Router";
//const char* password = "WT2A84tx";

const char* ssid = "VM647500-2G";
const char* password = "kmyhvyay";


void forwards(long interval);
void backwards(long interval);
void turn_right(long interval);
void turn_left(long interval);
void stop();


//-----------------------------------------

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  /*
  if(sizeof(data) > 0)
  {
    int szArr = sizeof(data)/sizeof(char);
    for(int i = 0; i < szArr; i ++)
      {
        Serial.println(data[i]);
      }
  }
  */
if (type == WStype_TEXT)
  {
    char* data;
    data = reinterpret_cast<char*>(payload);
    
    Serial.println(data);
    if(data[0] == 'f')
    {
      Serial.println("forwards");
      forwards(1000);
    }
    else if(data[0] == 'l')
    {
      Serial.println("left");
      turn_left(1000);
    }
    else if(data[0] == 'r')
    {
      Serial.println("right");
      turn_right(1000);
    }
  }
}

//-----------------------------------------
void setup()
{
  
  Serial.begin(115200);

  pinMode(speedL, OUTPUT);
  pinMode(dirL, OUTPUT);
  
  pinMode(speedR, OUTPUT);
  pinMode(dirR, OUTPUT);

wiFiMulti.addAP("UAL-IoT", "u4110t2016");

  while (wiFiMulti.run() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }


  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
}


void loop()
{
  webSocket.loop();
}



//----------MOVEMENT FUNCTIONS-------------------------------
void forwards(long interval) 
{
  analogWrite(speedR, Speed);
  analogWrite(speedL, Speed);
      
  digitalWrite(dirR, HIGH);
  digitalWrite(dirL, HIGH);

  currentTime = millis();
  previousTime = millis();
    
  while(timer <= interval)
  {
    currentTime = millis();
    int delta = currentTime - previousTime;
    timer += delta;
    previousTime = currentTime;
    yield(); // updates wdt
  }
  
  stop();
  timer = 0;
}
//-----------------------------------------
void backwards(long interval)
{
  analogWrite(speedR, Speed);
  analogWrite(speedL, Speed);
    
  digitalWrite(dirR, LOW);
  digitalWrite(dirL, LOW);

  currentTime = millis();
  previousTime = millis();
  
  while(timer <= interval)
  {
    currentTime = millis();
    int delta = currentTime - previousTime;
    timer += delta;
    previousTime = currentTime;
    yield(); // updates wdt
  }
  
  stop();
  timer = 0;
}
//-----------------------------------------
void turn_right(long interval)
{
  analogWrite(speedR, Speed);
  analogWrite(speedL, Speed);
  
  digitalWrite(dirR, LOW);
  digitalWrite(dirL, HIGH);

  currentTime = millis();
  previousTime = millis();
  
  while(timer <= interval)
  {
    currentTime = millis();
    int delta = currentTime - previousTime;
    timer += delta;
    previousTime = currentTime;
    yield(); // updates wdt
  }
  
  stop();
  timer = 0;

}
//-----------------------------------------
void turn_left(long interval)
{
  analogWrite(speedR, Speed);
  analogWrite(speedL, Speed);
  
  digitalWrite(dirR, HIGH);
  digitalWrite(dirL, LOW);
  
  currentTime = millis();
  previousTime = millis();
  
  while(timer <= interval)
  {
    currentTime = millis();
    int delta = currentTime - previousTime;
    timer += delta;
    previousTime = currentTime;
    yield(); // updates wdt
  }
  
  stop();
  timer = 0;
}
//-----------------------------------------
void stop()
{
 analogWrite(speedR, 0);
 analogWrite(speedL, 0);
}
 
