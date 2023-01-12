//variables
byte seven_seg_digits[14] = {
  B01111010,//D
  B10011100,//C
  B00111110,//B
  B11101110,//A
  B11100110,//9
  B11111110,//8
  B11100000,//7
  B10111110,//6
  B10110110,//5
  B01100110,//4
  B11110010,//3
  B11011010,//2
  B01100000,//1
  B11111100,//0
}; //Number array

const int latchPin = 9, clockPin = 10, dataPin = 8;



void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

//write digit / letter
void sevenSegWrite(byte digit)
{
  digitalWrite(latchPin, LOW);//set to low potential, ready to send in data
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);//write in the digit into LED
  digitalWrite(latchPin, HIGH);//set to high potential, can't send in data now
  
}

void loop() 
{
  for(byte digit = 14; digit > 0; --digit)//write all digits, D to 0
  {
    delay(1000);
    sevenSegWrite(digit-1);
  }

  delay(6000);//suspend for 6 secs
  // put your main code here, to run repeatedly:

}
