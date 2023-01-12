//In this code, I'm trying to simulate a siren going off.
//I'm using a passive buzzer as the noise creator



//variables
const int low = 150, high = 750;
const int buzzer = 12;
int wait;

//setup
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer, OUTPUT);

}

//loop
void loop() {
  // put your main code here, to run repeatedly:
  wait = 500;
  for(int x = 0; x < 3; x++)//repeat 3 times
  {
    tone(buzzer, high, wait);
    delay(500);
    tone(buzzer,low, wait);
    delay(500);
  }
  noTone(buzzer);
  delay(3000);

}
