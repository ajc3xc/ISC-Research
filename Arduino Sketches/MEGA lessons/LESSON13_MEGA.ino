#include <Keypad.h>

const byte ROWS = 4, COLS = 4;
char hexaKeys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const byte rowPins[ROWS] = {9,8,7,6};
const byte colPins[COLS] = {5,4,3,2};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS );


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char customKey = customKeypad.getKey();
  if(customKey)
  {
    Serial.println(customKey);
  }

}
