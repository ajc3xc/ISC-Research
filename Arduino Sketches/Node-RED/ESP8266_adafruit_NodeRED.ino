#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       ""
#define WLAN_PASS       ""

/************************* MQTT Setup *********************************/

#define MQTT_SERVER      ""
#define MQTT_SERVERPORT  8883
#define MQTT_USERNAME    ""
#define MQTT_PASSWORD         ""

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

// This is the sha-1 fingerprint of the current certificate for <your-id>.s1.eu.hivemq.cloud (It's a wildcard certificate, so should be the same for all deployments) I'm not sure at this point if this will change once the letsencrypt certificate is updated
static const char *fingerprint PROGMEM = "EA 59 CA BD 80 01 01 E8 12 58 B6 4E A3 59 2B 5A 6E 60 BC F5";

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish sensor = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/sensors/moisture/test1");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

int rawSensorValue = 0;
int normalizedValue = 0;

void setup() {
  Serial.begin(115200);
}

void wakeupCallback() { }

void loop() {
  // Connect to WiFi access point.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  client.setFingerprint(fingerprint);

  
  // mqtt.subscribe(&onoffbutton);

  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  Adafruit_MQTT_Subscribe *subscription;
  /*while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got onoff: "));
      Serial.println((char *)onoffbutton.lastread);
    } else if (subscription == &errors) {
      Serial.print(F("ERROR: "));
      Serial.println((char *)errors.lastread);
    } else if (subscription == &throttle) {
      Serial.println((char *)throttle.lastread);
    }
  }*/

  rawSensorValue = analogRead(A0);
  // Max dryness = 850, max wetness = 450, normalize to 0 - 100 range
  normalizedValue = (850 - min(rawSensorValue, 850)) / 4;
  
  // Now we can publish stuff!
  Serial.print(F("\nSending moisture, raw: "));
  Serial.print(rawSensorValue);
  Serial.print(", normalized: ");
  Serial.println(normalizedValue);
  
  Serial.print(F("Publishing... "));
  if (! sensor.publish(normalizedValue)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
 mqtt.disconnect();
 Serial.println("Sleeping...");
 // TODO: deepsleep doesn't work for me(?) fix this somehow?
 //ESP.deepSleep(60e6);

 // Timed Light Sleep instead for now
 //extern os_timer_t *timer_list;
 //timer_list = nullptr;  // stop (but don't disable) the 4 OS timers
 WiFi.mode(WIFI_OFF);
 wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
 wifi_fpm_set_wakeup_cb(wakeupCallback);
 wifi_fpm_open();
 wifi_fpm_do_sleep(60E6);
 delay(60e3 + 1);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
