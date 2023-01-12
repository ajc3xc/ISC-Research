#include "SR04.h"
#define TRIG_PIN 12
#define ECHO_PIN 11
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);
long a;
int LedPin = 13;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  a = sr04.Distance();
  Serial.print(a);
  Serial.println("cm");
  if(a < 10)   digitalWrite(LedPin, HIGH);
  delay(1000);
  digitalWrite(LedPin, LOW);
}
