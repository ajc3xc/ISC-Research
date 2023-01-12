const byte sensorPin = 6;
byte lastState = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensorPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte currentState = !digitalRead(sensorPin);

  if(currentState != lastState)
  {
    lastState = currentState;
    Serial.println(currentState);
  }
}
