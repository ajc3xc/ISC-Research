#define E1 5
#define I1 3
#define I2 4
int i;


void setup() {
  // put your setup code here, to run once:
  pinMode(E1, OUTPUT);
  pinMode(I1, OUTPUT);
  pinMode(I2, OUTPUT);
}

void backForward()
{
 for(i=0;i<5;i++)
  {
    digitalWrite(I1, HIGH);//one way
    digitalWrite(I2, LOW);
    delay(1000);
    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
    delay(1000);
  }
  digitalWrite(E1, LOW);
  delay(3000); 
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("One way, then reverse");
  digitalWrite(E1, HIGH); //enable on
  backForward();
  backForward();
}
