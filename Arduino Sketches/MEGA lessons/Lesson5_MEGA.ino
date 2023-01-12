const int Do = 262, Re = 294, Mi = 330, Fa = 349, Sol = 392, La = 440, Ti = 494, Do2 = 524;
const int buzzer = 12;
int wait = 0;


void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  wait = 500;
  tone(buzzer, Do, wait);
  delay(750);
  tone(buzzer, Re, wait);
  delay(750);
  tone(buzzer, Mi, wait);
  delay(750);
  tone(buzzer, Fa, wait);
  delay(750);
  tone(buzzer, Sol, wait);
  delay(750);
  tone(buzzer, La, wait);
  delay(750);
  tone(buzzer, Ti, wait);
  delay(750);
  tone(buzzer, Do2,wait);
  delay(750);
  noTone(buzzer);
  delay(5000);

}
