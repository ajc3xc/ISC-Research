#include <Stepper.h>
#define STEPS 32

volatile boolean TurnDetected, rotationdirection;
const int PinCLK = 2, PinDT = 3, PinSW = 4;
int RotaryPosition = 0, PrevPosition, StepsToTake;
Stepper small_stepper(STEPS, 8, 10, 9, 11);

void isr()
{
  delay(4); //Debouncing
  if(digitalRead(PinCLK))
  {
    rotationdirection = digitalRead(PinDT);
  }
  else      rotationdirection = !digitalRead(PinDT);
  TurnDetected = true;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  digitalWrite(PinSW, HIGH); //pull up resistor
  attachInterrupt(0,isr,FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  small_stepper.setSpeed(700);
  if(!digitalRead(PinSW)) //if button pressed
  {
    if(RotaryPosition != 0)
    {
      small_stepper.step(-(RotaryPosition*50));
      RotaryPosition = 0;
    }
  }

  if(TurnDetected)
  {
    PrevPosition = RotaryPosition;
    if(rotationdirection)   RotaryPosition--;
    else    RotaryPosition++;
    TurnDetected = false;
  }

  if( (PrevPosition+1)==RotaryPosition)
  {
    StepsToTake = 50;
    small_stepper.step(StepsToTake);
  }
}
