const int SW = 2, Xpin = A0, Ypin = A1;
const int redPin = 12, bluePin = 13;

void setup() {
  //setup input
  pinMode(SW, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  digitalWrite(SW, HIGH);

  //setup serial monitor
  Serial.begin(9600);
}

//testing rgb color value with light
void setColor(int redValue, int blueValue)
{
  analogWrite(redPin, redValue);
  analogWrite(bluePin, blueValue);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Switch: ");
  Serial.println( digitalRead(SW) );
  Serial.print("X-axis: ");
  Serial.println( analogRead(Xpin) );
  Serial.print("Y-axis: ");
  Serial.println( analogRead(Ypin) );
  Serial.println("");
  //Blue is up down, Red is left to right
  setColor( analogRead(Xpin)*255/1023, analogRead(Ypin)*255/1023 );
  delay(2000);

}
