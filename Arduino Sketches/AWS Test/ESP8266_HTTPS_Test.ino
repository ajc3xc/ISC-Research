/* ESP8266 HTTPS to AWS IoT
 * TESTED TO WORK ON ARDUINO 1.8.16 WITH ESP8266 BOARD PACKAGE VERSION 2.74
 *  Tested with ESP8266 Board Manager 2.74, version 3+ may not work, may need BEARSSL cert adjustment
 * Authors: Anthony Elder, Earle F. Philhower, III, Rui Santos and other sketches I pilfered for inspiration.
 * Sketch Modified by Stephen Borsay for www.udemy.com and my AWS IoT Live Workshops
 * License: Apache License v2
 * github.com/sborsay
 * Add in Char buffer utilizing sprintf to dispatch JSON data to AWS IoT Core
 * Use and replace your own SID, PW, AWS Account Endpoint, Client cert, private cert, x.509 CA root Cert
 
  ----------------------------
 
 ### Compile this only if using ESP8266 board manager version below 3.0, not backwards comparable
 
 ## Tools--> Board--> Board Manager-->ESP8266 BELOW version 3.0 by ESP8266 Community
 
 ### Older ESP8266 sketch is not forward compatible
 
 --------------------------------
 * 
 * also
 * github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 * 
 * references
 *docs.aws.amazon.com/iot/latest/developerguide/http.html#httpurl
 *github.com/espressif/arduino-esp32/blob/master/libraries/WiFiClientSecure/examples/WiFiClientSecure/WiFiClientSecure.ino
 *github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_ServerClientCert/BearSSL_ServerClientCert.ino
 */
#include <ESP8266WiFi.h>
extern "C" {
#include "libb64/cdecode.h"
}

#include <WiFiClientSecure.h> 

const char* ssid = "";  //your WiFi network
const char* password = "";  //Your WiFi password


const char *host = ""; //AWS IoT Core--> Settings (Device data endpoint)
const int httpsPort = 8443; //typical values are HTTPS= 443, MQTT=8883, and HTTP = 80 but AWS IoT Core uses 84443 for HTTP(s) Certificate secured
//AWS IoT Topic Subscribe to outTopic" in MQTT test client
const char *uri = "/topics/outTopic?qos=1";  //see https://docs.aws.amazon.com/iot/latest/developerguide/http.html


// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(

)EOF"; 
//-----END CERTIFICATE-----

// xxxxxxxxxx-private.pem.key
const String privatePemKey = \
//-----BEGIN RSA PRIVATE KEY-----
R"EOF(

)EOF";
//-----END RSA PRIVATE KEY-----

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = \
//-----BEGIN CERTIFICATE-----
R"EOF(

)EOF";
//-----END CERTIFICATE-----

WiFiClientSecure wiFiClient;

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
}


void loop() {

  Serial.println(host);

  wiFiClient.setTimeout(15000); // 15 Seconds
  delay(1000);
  
  Serial.print("HTTPS Connecting\n");
  int r=0; //retry counter
  while((!wiFiClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      Serial.print(".");
      r++;
  }
  if(r==30) {
    Serial.println("Connection failed");
  }
  else {
    Serial.println("Connected to web");
  }
  

  char fakeData[256]; 
  float temperature =  random(25,99); //fake number range, adjust as you like  
  float humidity =  random(50,99);  
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, temperature, humidity); // JSON Object with escapes
                                                                                                           //Use %d instead of %f to save memory space

  char postStr[64];
  sprintf(postStr, "POST %s HTTP/1.1", uri);  // put together the string for HTTP POST
  
  wiFiClient.println(postStr);
  wiFiClient.print("Host: ");
  wiFiClient.println(host);
  wiFiClient.println("Content-Type: application/json");
  wiFiClient.print("Content-Length: "); 
  wiFiClient.println(strlen(fakeData));
  wiFiClient.println();    // extra `\r\n` to separate the http header and http body
  wiFiClient.println(fakeData);

  Serial.println(fakeData);
  Serial.println("request sent");
  Serial.print("Heap space remaining: ");
  Serial.println(ESP.getFreeHeap());  //Libraby macro to observe remaining heap size in bytes.
  delay(10000);  // myClient.stop must be placed after some delay
  wiFiClient.stop();  //prevents heap colliding with stack, remember, no automatic garbage collection in C/C++
                      //This is a problem with the ESP32 but not usually the ESP8266 for some reason
                      //Compiler optimization may be better at freeing out of scope object memory in the ESP8266 than the ESP32

}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0,"pool.ntp.org", "time.nist.gov");

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
