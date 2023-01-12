//variables
const int adc_id = 0;
int HistoryValue = 0;
char printBuffer[128];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(adc_id);
  if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
  {
    sprintf(printBuffer,"ADC%d level is %d\n",adc_id, value);
    Serial.print(printBuffer);
    HistoryValue = value;
  }
}
