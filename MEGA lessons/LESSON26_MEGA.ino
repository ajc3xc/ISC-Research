#include <DS3231.h>
#include <Wire.h>
#include <TimeLib.h>
DS3231 clock;
RTCDateTime dt;
//this is too annoying. I don't have time to get the clock working

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.println("Initialize DS3231");
  clock.begin();
  clock.setDateTime(__DATE__,__TIME__);
}

void loop() {
  // put your main code here, to run repeatedly:
  dt = clock.getDateTime();
  Serial.print("Long number format:   ");
  Serial.println(clock.dateFormat("d-m-Y H:i:s", dt) );
  Serial.print("Long format with month name: ");
  Serial.println(clock.dateFormat("d F Y H:i:s", dt));
  Serial.print("Short format with 12h mode: ");
  Serial.println(clock.dateFormat("jS M y h:i:a", dt) );
  Serial.print("Today is:     ");
  Serial.print(clock.dateFormat("l,z", dt) );
  Serial.println("days of the year.");
  Serial.print("Actual month has:       ");
  Serial.print(clock.dateFormat("t", dt) );
  Serial.println("days.");
  Serial.print("Unixtime:     ");
  Serial.println(clock.dateFormat("U", dt) );
  Serial.println();
  delay(5000);
}
