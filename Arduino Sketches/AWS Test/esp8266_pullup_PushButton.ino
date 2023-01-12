const int buttonPin = D2;
int buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);

  Serial.print("Button 1 State: ");
  if(buttonState==HIGH) Serial.println("on");
  else Serial.println("off");
  delay(1000);
}
