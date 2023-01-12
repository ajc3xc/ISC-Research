#include <LedControl.h>
#include <LiquidCrystal.h>
#include "LedControl.h"

LedControl lc = LedControl(12,10,11,1);

unsigned long delaytime1 = 2000;
unsigned long delaytime2 = 100;

void setup() {
  // put your setup code here, to run once:
  lc.shutdown(0,false); //wakeup call
  lc.setIntensity(0,8); //set brightness to medium
  lc.clearDisplay(0); //Clear display
}

//say my name, say my name
void Adam()
{
  byte A[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte d[5]={B00111000,B01000100,B10000010,B10000010,B11111110};
  byte a[5]={B01111110,B10001000,B10001000,B10001000,B01111110};
  byte m[3] = {B11111110,B01000000, B00100000};

  //--------------display----------------//
  
  //A
  lc.setRow(0,0,A[0]);
  lc.setRow(0,1,A[1]);
  lc.setRow(0,2,A[2]);
  lc.setRow(0,3,A[3]);
  lc.setRow(0,4,A[4]);
  delay(delaytime1);
  
  //d
  clearDisplay();
  for(int x = 0; x<5; x++)
  {
    lc.setRow(0,x,d[x]);
  }
  delay(delaytime1);

  //a
  clearDisplay();
  for(int x = 0; x<5; x++)
  {
    lc.setRow(0,x,a[x]);
  }
  delay(delaytime1);

  //m
  clearDisplay();
  lc.setRow(0,0,m[0]);
  lc.setRow(0,1,m[1]);
  lc.setRow(0,2,m[2]);
  lc.setRow(0,3,m[1]);
  lc.setRow(0,4,m[0]);
  delay(delaytime1);

  //clear
  clearDisplay();
  delay(5000);
  
}

void clearDisplay()
{
  for(int x = 0; x<8; x++)  lc.setRow(0,x,0);  
}

void loop() {
  // put your main code here, to run repeatedly:
  Adam(); //this took too long to make
  
}
