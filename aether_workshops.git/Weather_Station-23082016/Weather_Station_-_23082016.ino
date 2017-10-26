// www.arduinesp.com 
//
// Plot DTH11 data on thingspeak.com using an ESP8266 
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com
 
 
#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel's thingspeak API key, 
String apiKey = "P6TOSFKO6D8GZGB3";


//const char* ssid = "401Router";
//const char* password = "WT2A84tx";

//const char* ssid = "VM647500-2G";
//const char* password = "kmyhvyay";

const char* ssid = "Rosemary's iPhone";
const char* password = "a7wgx9brdk72w";
 
const char* server = "api.thingspeak.com";
#define DHTPIN D4 // what pin we're connected to
 
DHT dht(DHTPIN, DHT11,15);
WiFiClient client;

int sensorPin = A0;  //input LDR and rainfall
int enableRain = D6;    //enable readin rainfall
int enableLdr = D8;    //enable reading LDR

//int rain = 0; //variable to store value from rainfall
//int ldr = 0; //variable to store value from LDR

int led = D1;

unsigned long currentTime;
unsigned long previousTime;
long interval = 10000;
 
void setup() { 

  pinMode(enableRain, OUTPUT);
  pinMode(enableLdr, OUTPUT);

  pinMode(led, OUTPUT);
                 
  Serial.begin(9600);
  delay(10);
  dht.begin();
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
 
 
void loop() {

  currentTime = millis();

  if(currentTime - previousTime > interval){
  previousTime = currentTime;

 //---------DHT11 TEMPERATURE + HUMIDITY
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    Serial.println(".........................................");
    Serial.println();
    return;
  }
     
     Serial.print("Temperature:     ");
     Serial.print(t);
     Serial.println(" degrees Celcius");
     Serial.print("Humidity:        "); 
     Serial.print(h);
     Serial.println(" %");

//----------DEW POINT
  
  double gamma = log(h/100) + ((17.62*t) / (243.5+t));
  double dp = 243.5*gamma / (17.62-gamma);
  
  Serial.print("Dew point:       ");
  Serial.print(dp);
  Serial.println(" degrees Celcius");

//----------RAIN
  
  digitalWrite(enableRain, HIGH);
  
  int rain = analogRead(sensorPin);
  rain = constrain(rain, 97, 424);
  int rainReal = map(rain, 97, 424, 100, 0);

  Serial.print("Rainfall:        "); 
  Serial.print(rainReal);
  Serial.println(" %");  

  digitalWrite(enableRain, LOW);
  delay(100);
  
//----------LDR
  digitalWrite(enableLdr, HIGH);

  int ldr = analogRead(sensorPin);
  ldr = constrain(ldr, 300, 1000);
  int ldrReal = map(ldr, 300, 1000, 0, 100);

  Serial.print("Light Intensity: ");
  Serial.print(ldrReal);
  Serial.println(" %");
  
  digitalWrite(enableLdr, LOW);

 //-------------THINGSPEAK

  digitalWrite(led, HIGH);
  
  if (client.connect(server,80)) {  //   "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
           postStr +="&field1=";
           postStr += String(t);
           postStr +="&field2=";
           postStr += String(h);
           postStr += "&field3=";
           postStr += String(dp);
           postStr += "&field4=";
           postStr += String(rainReal);
           postStr += "&field5=";
           postStr += String(ldrReal);
           postStr += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n"); 
     client.print("Host: api.thingspeak.com\n"); 
     client.print("Connection: close\n"); 
     client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n"); 
     client.print("Content-Type: application/x-www-form-urlencoded\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
  
  }
  client.stop();

  digitalWrite(led, LOW);
   
  Serial.println(".........................................");
  Serial.println();    
  // thingspeak needs minimum 15 sec delay between updates
  }
  delay(10);  
}

