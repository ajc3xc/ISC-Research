//variables
int latchPin = 4;
int clockPin = 5;
int dataPin = 2;
int currentLed = 0;
byte leds = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  leds = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  leds = 0;

  //changes to each LED
  if(currentLed>=7)
  {
    currentLed = 0;
  }
  else
  {
    currentLed++;
  }

  bitSet(leds, currentLed);
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, leds);
  digitalWrite(latchPin, HIGH);
  delay(100);

}
