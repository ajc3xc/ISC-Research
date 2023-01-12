#include <Servo.h>
//variables

//objects
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int pos = 0; pos <= 180; pos++)//can only turn up to 180 degrees
  {
    myservo.write(pos);
    delay(15);
  }
  for(int pos = 180; pos >= 0; pos--)
  {
    myservo.write(pos);
    delay(15);
  }
}
