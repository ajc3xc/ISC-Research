/* ESP8266 AWS IoT
 *  
 * ----------------------------
 *
 * Compile this only if using ESP8266 version 3+, not backwards compatable
 *
 * Tools--> Board--> Board Manager-->ESP8266 version 3+ by ESP8266 Community
 *
 * Older ESP8266 sketch is not forward compatible
 *
 * --------------------------------
 *
 *
 * Simplest possible example (that I could come up with) of using an ESP8266 with AWS IoT.
 * No messing with openssl or spiffs just regular pubsub and certificates in string constants
 *
 * This is working as at 3rd Aug 2019 with the current ESP8266 Arduino core release:
 * SDK:2.2.1(cfd48f3)/Core:2.5.2-56-g403001e3=20502056/lwIP:STABLE-2_1_2_RELEASE/glue:1.1-7-g82abda3/BearSSL:6b9587f
 *
 * Author: Anthony Elder
 * License: Apache License v2
 *  Also see:  https://github.com/Ameba8195/Arduino/blob/master/hardware_v2/libraries/MQTTClient/examples/amazon_awsiot_basic/amazon_awsiot_basic.ino
 * Sketch Modified by Stephen Borsay for www.udemy.com/course/exploring-aws-iot/
 * https://github.com/sborsay
 * Add in EOF certificate delimiter
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * First 9 chars of certs obfusicated, use your own, but you can share root CA / x.509 until revoked
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "<Your-WiFi-Network-Here>";
const char* password = "<Your-Password-Here>";

const char* awsEndpoint = "<YOURACCOUNTID-ats.iot.REGION.amazonaws.com>"; //AWS IoT Core--> Settings (Device data endpoint)

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key:

// xxxxxxxxxx-certificate.pem.crt
static const char certificatePemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// xxxxxxxxxx-private.pem.key
static const char privatePemKey[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)EOF";

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
static const char caPemCrt[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

BearSSL::X509List client_crt(certificatePemCrt);
BearSSL::PrivateKey client_key(privatePemKey);
BearSSL::X509List rootCert(caPemCrt);

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  wiFiClient.setClientRSACert(&client_crt, &client_key);
  wiFiClient.setTrustAnchors(&rootCert);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

  //If you need to increase buffer size, change MQTT_MAX_PACKET_SIZE in PubSubClient.h
   char fakeData[128];

  float var1 =  random(0,120); //fake number range, adjust as you like
  float var2 =  random(0,100);
  sprintf(fakeData,  "{\"uptime\":%lu,\"temperature\":%f,\"humidity\":%f}", millis() / 1000, var1, var2);
  
  if (millis() - lastPublish > 5000) {
    boolean rc = pubSubClient.publish("outTopic", fakeData);
    Serial.print("Published, rc=");
    Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(fakeData);
    lastPublish = millis();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthing");
    }
    Serial.println(" connected");
    pubSubClient.subscribe("inTopic");
  }
  pubSubClient.loop();
}

void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
