  const int RedPin = 3;
  const int BluePin = 6;
  const int GreenPin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(RedPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Green LED is non functional
  setColor(255,0,0);
  delay(2500);
  setColor(0,255,0);
  delay(2500);
  setColor(0,0,255);
  delay(2500);
  setColor(255,255,255);
  delay(2500);
}

void setColor(int redValue, int blueValue, int greenValue)
{
  analogWrite(RedPin, redValue);
  analogWrite(GreenPin, greenValue);
  analogWrite(BluePin, greenValue);
}
