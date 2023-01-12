

//variables
const int latch = 9, clock = 10, data = 8;

//7 segment decoding
unsigned char table[] = 
{0x3f, 0x06, 0x5b, 0x4f, 0x66,0x6d, 0x7d, 0x07,0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00};



void setup() {
  // put your setup code here, to run once:
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
}

void Display(unsigned char num)
{
  digitalWrite(latch, LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int x = 1; x <= 15; x++)
  {
    Display(x);
    delay(2000);
  }

}
