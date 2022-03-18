
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Espalexa.h>
#include<PubSubClient.h>
#include <WiFiManager.h>


#define clientid "alexahomeautomation"

const char *mqttserver = "117.196.244.184";
int port = 1883;


WiFiClient espclient;
PubSubClient client(espclient);

// define the GPIO connected with Relays
#define RelayPin1 16  //D1
#define RelayPin2 5  //D2
#define RelayPin3 4 //D7
#define RelayPin4 0 //D5
#define RelayPin5 2 //D6
#define RelayPin6 14 //D7
#define RelayPin7 12 //D5
#define RelayPin8 13//D6


// prototypes
boolean connectWifi();

//callback functions
void firstLightChanged(uint8_t brightness);
void secondLightChanged(uint8_t brightness);
void thirdLightChanged(uint8_t brightness);

void fifthLightChanged(uint8_t brightness);
void sixthLightChanged(uint8_t brightness);
void seventhLightChanged(uint8_t brightness);
void eighthLightChanged(uint8_t brightness);

// WiFi Credentials
const char* ssid = "sandeep";
const char* password = "22122012";

// device names
String Device_1_Name = "frontlight";
String Device_2_Name = "backlight";
String Device_3_Name = "centrelight";
String Device_5_Name = "fan1";
String Device_6_Name = "fan2";
String Device_7_Name = "fan3";
String Device_8_Name = "fan4";

int fl,bl,cl,f1,f2,f3,f4;

boolean wifiConnected = false;

Espalexa espalexa;

void reconnect()
{
  while(!client.connected())
  {
    Serial.println("Attempting the MQTT Connection");
    if(client.connect(clientid))
    {
      Serial.println("MQTT Connected");
    }
  }
}
void setup()
{
  fl,bl,cl,f1,f2,f3,f4=0;
  Serial.begin(115200);
  client.setServer(mqttserver,port);
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  pinMode(RelayPin5, OUTPUT);
  pinMode(RelayPin6, OUTPUT);
  pinMode(RelayPin7, OUTPUT);
  pinMode(RelayPin8, OUTPUT);

  digitalWrite(RelayPin1, 1);
  digitalWrite(RelayPin2, 1);
  digitalWrite(RelayPin3, 1);
  digitalWrite(RelayPin4, 1);
  digitalWrite(RelayPin5, 1);
  digitalWrite(RelayPin6, 1);
  digitalWrite(RelayPin7, 1);
  digitalWrite(RelayPin8, 1);
  
  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected)
  {
    // Define your devices here.
    espalexa.addDevice(Device_1_Name, firstLightChanged); //simplest definition, default state off
    espalexa.addDevice(Device_2_Name, secondLightChanged);
    espalexa.addDevice(Device_3_Name, thirdLightChanged);

    espalexa.addDevice(Device_5_Name, fifthLightChanged);
    espalexa.addDevice(Device_6_Name, sixthLightChanged);
    espalexa.addDevice(Device_7_Name, seventhLightChanged);
    espalexa.addDevice(Device_8_Name, eighthLightChanged);
    espalexa.begin();
  }
  else
  {
    while (1)
    {
      Serial.println("Cannot connect to WiFi. Please check data and reset the ESP.");
      delay(2500);
    }
  }
}
char msg[40];
void loop()
{
  if(!client.connected())
  {
    reconnect();
  }
  espalexa.loop();
  String homestatus;
  homestatus="alexa,"+String(fl)+","+String(cl)+","+String(bl)+","+String(f1)+","+String(f2)+","+String(f3)+","+String(f4);
  homestatus.toCharArray(msg,40);
  client.publish("iotroom/alexa",msg);
  delay(1000);
}

//our callback functions
void firstLightChanged(uint8_t brightness)
{
  //Control the device
  if (brightness == 255)
    {
      digitalWrite(RelayPin1, 0);
      digitalWrite(RelayPin2, 0);
      Serial.println("Device1 ON");
      fl=1;
    }
  else
  {
    digitalWrite(RelayPin1, 1);
    digitalWrite(RelayPin2, 1);
    Serial.println("Device1 OFF");
    fl=0;
  }
}

void secondLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin3, 0);
      Serial.println("Device2 ON");
      bl=1;
    }
  else
  {
    digitalWrite(RelayPin3, 1);
    Serial.println("Device2 OFF");
    bl=0;
  }
}

void thirdLightChanged(uint8_t brightness)
{
  //Control the device  
  if (brightness == 255)
    {
      digitalWrite(RelayPin4, 0);
      Serial.println("Device3 ON");
      cl=1;
    }
  else
  {
    digitalWrite(RelayPin4, 1);
    Serial.println("Device3 OFF");
    cl=0;
  }
}



void fifthLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin5, 0);
      Serial.println("Device5 ON");
      f1=1;
    }
  else
  {
    digitalWrite(RelayPin5, 1);
    Serial.println("Device5 OFF");
    f1=0;
  }
}

void sixthLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin6, 0);
      Serial.println("Device6 ON");
      f2=1;
    }
  else
  {
    digitalWrite(RelayPin6, 1);
    Serial.println("Device6 OFF");
    f2=0;
  }
}

void seventhLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin7, 0);
      Serial.println("Device7 ON");
      f3=1;
    }
  else
  {
    digitalWrite(RelayPin7, 1);
    Serial.println("Device7 OFF");
    f3=0;
  }
}


void eighthLightChanged(uint8_t brightness)
{
  //Control the device 
  if (brightness == 255)
    {
      digitalWrite(RelayPin8, 0);
      Serial.println("Device8 ON");
      f4=1;
    }
  else
  {
    digitalWrite(RelayPin8, 1);
    Serial.println("Device8 OFF");
    f4=0;
  }
}



// connect to wifi  returns true if successful or false if not
boolean connectWifi()
{
  boolean state;


   WiFi.mode(WIFI_STA);
  WiFiManager wm;
//  bool res;
  state = wm.autoConnect("alexa-home-AP");
  if(!state) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  Serial.print("\n WiFi Connected with IP:");
  Serial.println(WiFi.localIP());
  return state;
}
