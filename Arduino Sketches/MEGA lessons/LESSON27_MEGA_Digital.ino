
//This only stays on. For some reason, if I write the code itself it doesn't turn on
//This is fucking confusing. This doesn't even work.
const int Led=13, buttonpin = 3;//define LED port
int  val = 0;//define digital variable val
void  setup()
{
  pinMode(Led,OUTPUT);//define LED as a output port
  pinMode(buttonpin,INPUT);//define switch as a output port
  Serial.begin(9600);
}

void  loop()

{
  val=digitalRead(buttonpin);//read the value of the digital interface 3 assigned to val 
  if(val==HIGH)//when the switch sensor have signal, LED blink
  {
    digitalWrite(Led,HIGH);
  }
  else
  {
  digitalWrite(Led,LOW);
  }
  Serial.println(val); //This only prints HIGH (1)
  delay(500);
}
