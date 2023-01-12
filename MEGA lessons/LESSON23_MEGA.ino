#include <MPU6050.h>
#include <Wire.h>
//variables
const int MPU_addr=0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); //PWR_MGMT1_Register
  Wire.write(0); // wake up MPU_6050
  Wire.endTransmission(true);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3C); //Accel xout H
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);

  AcX = Wire.read()<<8|Wire.read(); //Accel xout H & L
  AcY = Wire.read()<<8|Wire.read(); //Accel yout H and L
  AcZ = Wire.read()<<8|Wire.read(); //Accel zout H & L
  Tmp = Wire.read()<<8|Wire.read(); //Temp out H & L
  GyX = Wire.read()<<8|Wire.read(); //Gyro xout H & L
  GyY = Wire.read()<<8|Wire.read(); //Gyro yout H & L
  GyZ  = Wire.read()<<8|Wire.read(); //Gyro zout H & L

  Serial.print("AcX = "); Serial.println(AcX);
  Serial.print("AcY = "); Serial.println(AcY);
  Serial.print("AcZ = "); Serial.println(AcZ);
  Serial.print("Tmp = "); Serial.println(Tmp);
  Serial.print("GyX = "); Serial.println(GyX);
  Serial.print("GyY = "); Serial.println(GyY);
  Serial.print("GyZ = "); Serial.println(GyZ);
  Serial.println("");
  delay(3000);
}
