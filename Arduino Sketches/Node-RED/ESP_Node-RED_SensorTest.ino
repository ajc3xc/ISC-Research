/*
 * Virtuino MQTT getting started example
 * Broker: HiveMQ (Secure connection)
 * Supported boards: ESP8266 / ESP32 
 * Created by Ilias Lamprou
 * Jul 13 2021
 */

//Well boys we did it. MQTT bs is no more

#ifdef ESP8266
 #include <ESP8266WiFi.h>  // Pins for board ESP8266 Wemos-NodeMCU
 #else
 #include <WiFi.h>  
#endif
 
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <time.h>

//sensor pins
#define analogPin A0
#define digitalPin D2
#define BUILTIN_LED D0

//---- WiFi settings
const char* ssid = "";
const char* password = "";

//---- MQTT Broker settings
const char* mqtt_server = ""; // replace with your broker url
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port =8883;
int buttonState;

 

WiFiClientSecure espClient;   // for no secure connection use WiFiClient instead of WiFiClientSecure 
//WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


//input and output topic(s)
const char* espTopic = "esp8266-inTopic";
const char* inputTopic1 = "esp8266-outTopic";




//change every time CA signed certificate changes on website
//then connection will work
static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkTCCA3mgAwIBAgISBOC9WlX5kAF663DhQv3mzzuyMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMjA3MDUyMDUyMjBaFw0yMjEwMDMyMDUyMTlaMCcxJTAjBgNVBAMT
HGU4N3YwZS5zdGFja2hlcm8tbmV0d29yay5jb20wdjAQBgcqhkjOPQIBBgUrgQQA
IgNiAAQaoljTjdJFQQZQdGM5QG4detX6h6u5/7/9ahneQWPoemchw52Up3QFFVfn
uBPLaRur7pPVRBO7WpHPOgufflWnfycTH1lDZns6HN/AvdUGeQemktybl42daaS1
srhzG4ajggJYMIICVDAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0lBBYwFAYIKwYBBQUH
AwEGCCsGAQUFBwMCMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYEFK4EIOHp342yhwUX
DFkbxuQIfmkNMB8GA1UdIwQYMBaAFBQusxe3WFbLrlAJQOYfr52LFMLGMFUGCCsG
AQUFBwEBBEkwRzAhBggrBgEFBQcwAYYVaHR0cDovL3IzLm8ubGVuY3Iub3JnMCIG
CCsGAQUFBzAChhZodHRwOi8vcjMuaS5sZW5jci5vcmcvMCcGA1UdEQQgMB6CHGU4
N3YwZS5zdGFja2hlcm8tbmV0d29yay5jb20wTAYDVR0gBEUwQzAIBgZngQwBAgEw
NwYLKwYBBAGC3xMBAQEwKDAmBggrBgEFBQcCARYaaHR0cDovL2Nwcy5sZXRzZW5j
cnlwdC5vcmcwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdwBGpVXrdfqRIDC1oolp
9PN9ESxBdL79SbiFq/L8cP5tRwAAAYHQWZUyAAAEAwBIMEYCIQD37xKfQ63zhsVc
kszGpQQ7Y2Owj68t0dECyeCmH6+TRwIhALm5b2fmuxWGgzD3lSp0YUoZI01H6n+y
HtZkHKEynKZYAHYAb1N2rDHwMRnYmQCkURX/dxUcEdkCwQApBo2yCJo32RMAAAGB
0FmVcwAABAMARzBFAiEAkYq1try3nOQJINom3TNXsIXBbeljzwyTBxo1rptXkYMC
IB6mp5kCrwtPqX7B7X3lsUUANXPZsdgnaI1Qb76eC8N3MA0GCSqGSIb3DQEBCwUA
A4IBAQAA92Q8piwVV8EsaRQvdOplrZ3r73/S8ncHQJGkww43W0+JhGa7HlEZSBNR
JFX2e1UbJMWjfe9Il34ISpxMpWgA8O5GxgKMU9nKiUC/YoKwRCk8e34v+eNHfa7U
AkQvIe71+RW4AJk7rQlrgz9+vpxEv63YIEFmQ0nonbgjFBz7f55AsNToRzNXxZxw
XrUBBo3tdT8SxsoVCvpcaQPq9z7/4HbSblJXye9DL931wyZPdDPmDW7QrzxajDAz
/4g9t/Y6/mgHq4z5V57RoDcFo/Rlk42rOUIxImniunOoRMBzfGIFmQi/HoG/yQDr
K3C2U2dEAURBmw2Rb/mtu16hBlbc
-----END CERTIFICATE-----
)EOF";


//wifi setup
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
}


//deal w/ mqtt connection
//The MOST difficult part
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(inputTopic1);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//setup
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);
  pinMode(BUILTIN_LED, OUTPUT); //setup built in led
  digitalWrite(BUILTIN_LED, HIGH);
  setup_wifi();
  //pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output


  #ifdef ESP8266
    espClient.setInsecure();
  #else   // for the ESP32
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


//'main' function
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  //---- example: how to publish sensor values every 5 sec
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    publishMessage();
    
  }
}


//deals w/ messages from broker. Can be used to mess w/ machines in future
void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  //any topic
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);

  
  //--- check the incomming message
  if( strcmp(topic,inputTopic1) == 0){
     if (incommingMessage.equals("1"))
     {
      digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
      Serial.println("LED turned on"); 
     }
     else if(incommingMessage.equals("0"))
     {
      digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
      Serial.println("LED turned off");
     }
  }
  
}


//send sensor data to Node-RED, via json
void publishMessage()
{
  buttonState = digitalRead(digitalPin);
  if(buttonState) buttonState = 0;
  else  buttonState = 1;  
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["voltage"] = analogRead(analogPin) * 3.3 / 1023;
  doc["on"] = digitalRead(digitalPin);
  doc["Device_ID"] = "esp8266(0)";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(espTopic, jsonBuffer);
}
