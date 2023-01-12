//sets var to a number
#define E1 5
#define I1 3
#define I2 4

void setup() {
  // put your setup code here, to run once:
  pinMode(E1, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(E1, 153); //half speed
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  delay(10000);

  //change direction
  digitalWrite(E1, LOW);
  delay(2000);
  analogWrite(E1, 255); //full speed
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  delay(10000);
}
