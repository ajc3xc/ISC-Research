
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 5
#define SS_PIN 53
MFRC522 mfrc522(SS_PIN, RST_PIN);
#define NEW UID{0xDE, 0xAD, 0xBE, 0xEF}; //set UID
MFRC522::MIFARE_Key key;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Warning: this example overwrites the UID of your UID changable card, use with care!"));
  for(byte i = 0; i < 8; i++)
  {
    key.keyByte[i] = 0xFF; //Factory preset key FFFFFFFFFFFFh
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() )
  {
    delay(1000);
    return;
  }

  //card is selected, uid and sak in
  //dump uid
  Serial.print(F("Card UID: "));
  for(byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc.uid.uidByte[i]<0x10 ? " 0":"");
    Serial.print(mfrc.uid.uidByte[i],HEX);
  }
  Serial.println();

  byte newUid[] = NEW_UID;
  if( mfrc522.MIFARE_SetUid(newUid, (byte)4, true) )
  {
    Serial.println(F("Wrote new UID to card"));
  }

  //Half PICC, reselect to DumpToSerial so not confused
  mfrc522.PICC_HaltA();
  if( !mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial() )    return;

  //Dump new memory contents
  Serial.println(F("New UID and contents: "));
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  delay(3000);
}
