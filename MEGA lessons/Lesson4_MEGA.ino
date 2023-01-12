int buzzer = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned char i;

  for(i = 0; i<60; i++)
  {
    digitalWrite(buzzer, HIGH);
    delay(1);
    digitalWrite(buzzer, LOW);  
  }

  for(int x = 0; x<80;x++)
  {
    digitalWrite(buzzer, HIGH);
    delay(3);
    digitalWrite(buzzer, LOW);
    delay(3);
  }

  delay(5000);
  

}
