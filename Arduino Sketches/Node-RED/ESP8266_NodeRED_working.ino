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

//---- WiFi settings
const char* ssid = "";
const char* password = "";

//---- MQTT Broker settings
const char* mqtt_server = ""; // replace with your broker url
const char* mqtt_username = "";
const char* mqtt_password = "";
const int mqtt_port =8883;

 

WiFiClientSecure espClient;   // for no secure connection use WiFiClient instead of WiFiClientSecure 
//WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];


int sensor1 = 0;
float sensor2 = 0;
int command1 =0;

const char* sensor1_topic= "sensor1";
const char*  sensor2_topic="sensor2";
const char* espTopic = "inTopic"; //this is the only one that matters. Delete the other topics later
//const char*  sensor2_topic="sensor3";

const char* command1_topic="command1";
//const char* command1_topic="command2";





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


//==========================================
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


//=====================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(command1_topic);   // subscribe the topics here
      //client.subscribe(command2_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//================================================ setup
//================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output


  #ifdef ESP8266
    espClient.setInsecure();
  #else   // for the ESP32
    espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  #endif
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


//================================================ loop
//================================================
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  //---- example: how to publish sensor values every 5 sec
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    publishMessage(espTopic, "Hello World",true); //publishes the value
    
  }
}

//=======================================  
// This void is called every time we have a message from the broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  //--- check the incomming message
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("1")) digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on 
     else digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off 
  }

   //  check for other commands
 /*  else  if( strcmp(topic,command2_topic) == 0){
     if (incommingMessage.equals("1")) {  } // do something else
  }
  */
}



//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Message publised ["+String(topic)+"]: "+payload);
}
