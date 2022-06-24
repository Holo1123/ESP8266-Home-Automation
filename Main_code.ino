/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL6PxbI2l-"
#define BLYNK_DEVICE_NAME "Template"
#define BLYNK_AUTH_TOKEN "LebNqMgyfELn3EQv3ub0RuwIx6t2aPZN"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN D4          // Digital pin 4
#define AcRelay D5          // Digital pin 5
#define GasRelay D1
#define LightRelay D6          // Digital pin 6
#define DHTTYPE DHT11 
#define Buzzer D2

int mq135 = A0; // smoke sensor is connected with the analog pin A0+
int mqR = 22000; 
int data = 0;

#define irPin1 D2
#define irPin2 D3
int count=0;
int i=1;
unsigned long int irTime1 = 0;
unsigned long int irTime2 = 0;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "X3";
char pass[] = "hassan123";

BlynkTimer timer;
DHT_Unified dht(DHTPIN, DHTTYPE);


// This function is called every time the Virtual Pin 0 state changes


BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();
  // Update state
  Blynk.virtualWrite(V1, value);
}
BLYNK_WRITE(V8)
{
  int Ac = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V8 Button value is");
  Serial.println(Ac);
  if(Ac==1){digitalWrite(AcRelay,HIGH);}
  else{digitalWrite(AcRelay,LOW);}
}
BLYNK_WRITE(V11)
{
  int Light = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V11 Button value is");
  Serial.println(Light);
  if(Light==1){digitalWrite(LightRelay,HIGH);}
  else{digitalWrite(LightRelay,LOW);} 
}
BLYNK_WRITE(V12)
{
  int TargetTemp = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V12 Button value is");
  Serial.println(TargetTemp);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float t = event.temperature;
  if(TargetTemp==0){}
  else if(TargetTemp==t){}
  else if(TargetTemp==t+1){}
  else if(TargetTemp==t-1){}
  //else if(TargetTemp==t+2){}
  //else if(TargetTemp==t-2){}
  else if(TargetTemp>t){digitalWrite(AcRelay,HIGH);}
  else if(TargetTemp<t){digitalWrite(AcRelay,LOW);}
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
void mq135readsend()
{
data = analogRead(mq135); 
  data =(data*675)/1023;
  Blynk.virtualWrite(V9, data); 
  Serial.print(data);
  if(data>500){Blynk.virtualWrite(V10, 1);
  digitalWrite(GasRelay,HIGH);
  digitalWrite(Buzzer,HIGH);
  delay(250);
  digitalWrite(Buzzer,LOW);
  delay(250);}
  else{Blynk.virtualWrite(V10, 0);
  digitalWrite(GasRelay,LOW);}
 }
void dhtreadsend()
{
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    float t = event.temperature;
    Blynk.virtualWrite(V6, t);  //V6 is for Temperature
    //Serial.print(F("Temperature: "));
    //Serial.print(event.temperature);
    //Serial.println(F("Â°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    //Serial.print(F("Humidity: "));
    //Serial.print(event.relative_humidity);
    //Serial.println(F("%"));
     float h = event.relative_humidity;
     Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  }
 

}
void setup()
{ 
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(AcRelay, OUTPUT);
  pinMode(LightRelay, OUTPUT);
  pinMode(GasRelay, OUTPUT);
  pinMode(Buzzer,OUTPUT);
  
  digitalWrite(AcRelay,HIGH);
  digitalWrite(LightRelay,HIGH);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
 dht.begin();

  timer.setInterval(1000L, myTimerEvent); // V2 used
  timer.setInterval(1000L, mq135readsend);  // V4 mq135
  timer.setInterval(1000L, dhtreadsend); // V5 H , V6 T
}

void loop()
{
  Blynk.run();
  timer.run();

}
