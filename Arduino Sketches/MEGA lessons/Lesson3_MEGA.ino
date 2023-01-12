  const int LedPin = 5, buttonApin = 9,buttonBpin = 8;
  byte leds = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LedPin, OUTPUT);
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(buttonBpin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(buttonApin)==LOW)
      digitalWrite(LedPin, HIGH);
  if(digitalRead(buttonBpin)==LOW)
      digitalWrite(LedPin, LOW);

}
