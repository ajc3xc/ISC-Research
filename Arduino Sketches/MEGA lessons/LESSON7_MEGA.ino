//variables
const int lightPin = 0, latchPin = 11, clockPin = 12, dataPin = 9;
int leds = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

}

//turn light off
void updateShiftRegister()
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int reading = analogRead(lightPin);
  int numLEDSLit = reading/57;
  if(numLEDSLit>8)  numLEDSLit = 8;
  leds = 0;

  for(int i = 0; i < numLEDSLit; i++)
  {
    leds = leds + (1<<i);
  }
  updateShiftRegister();

}
