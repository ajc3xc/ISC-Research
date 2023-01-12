int LedPin = 8;
int i;

void setup() {
  // put your setup code here, to run once:
  pinMode(LedPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  i = analogRead(5);
  if(i>1000) //greater than 2.5V
    digitalWrite(LedPin, HIGH);
  else
    digitalWrite(LedPin, LOW);
}
