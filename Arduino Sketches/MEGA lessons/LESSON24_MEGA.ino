//variables
const int led = 13, sensor = 7;
int state = LOW, val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(sensor);
  if(val==HIGH) //if sensor high
  {
    digitalWrite(led, HIGH); //turn led on
    delay(100); //delay 100ms
  
    if(state==LOW)
    {
        Serial.println("Motion detected!");
        state = HIGH;
    }
  }
  else
  {
    digitalWrite(led, LOW);
    delay(200);
    if(state==HIGH)
    {
        Serial.println("Motion stopped!");
        state = LOW;
    }
  }
}
