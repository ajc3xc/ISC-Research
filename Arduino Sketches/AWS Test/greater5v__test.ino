#define cellPin A0
float sensorValue;
float voltage;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

float getVolts()
{
  sensorValue = analogRead(cellPin);
  voltage = sensorValue * (9.0 / 1023.00) * (6);
  return voltage;
}

float getInput()
{
  sensorValue = analogRead(cellPin);
  voltage = sensorValue;
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println( "output MV: " + String( 5.0 * analogRead(cellPin)/1023.0 ) );
  delay(500);
}
