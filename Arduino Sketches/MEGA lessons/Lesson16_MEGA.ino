#include "Stepper.h"
#include "IRremote.h"
#define STEPS 32

//variables
int Steps2Take; //2048 = 1 rev
const int receiver = 12;

//object delcarations
Stepper small_stepper(STEPS, 8, 10, 9, 11);
IRrecv irrecv(receiver);
decode_results results;


void setup() {
  // put your setup code here, to run once:
  irrecv.enableIRIn();
  Serial.begin(9600);
}

void Revolution(int steps)
{
  small_stepper.setSpeed(500);
  Steps2Take = steps;
  small_stepper.step(Steps2Take);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if( irrecv.decode(&results) )//get signal
  {
   switch(results.value)
   {
      case 0xFF906F:  //VOL+
          Revolution(-2048); //CW. Opposite of what lesson said. Something wrong?
          Serial.println(1); //output on monitor
          break;
      case 0xFFA857:
          Revolution(2048); //CCW
          Serial.println(2); //output on monitor
          break;
  }
  irrecv.resume();
  }

}
