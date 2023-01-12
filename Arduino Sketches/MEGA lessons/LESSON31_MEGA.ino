//and so the journey must come to an end
const int rainPin = A0, greenPin = 8, redPin = 7;
int thresholdValue = 950;

void setup() {
  // put your setup code here, to run once:
  pinMode(rainPin, INPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorValue = analogRead(rainPin);
  Serial.print(sensorValue);
  if(sensorValue <950)
  {
    Serial.println(" - Doesn't need watering");
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
  else
  {
    Serial.println(" - This plant is thirsty!");
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
  }
  delay(1000);
}
