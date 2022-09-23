#include <Wire.h>
#define potpin A0

int pot;

byte x = 0;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
	Serial.begin(9600);
}

void loop()
{
  
  
  pot= analogRead(potpin);
  
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write("potantiometer is ");        // sends five bytes, with send loop
  
  Wire.write((pot>>2));              // sends one byte  
  //Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  Serial.println((pot>>2)); // 2^8
  Serial.println(pot); //2^10
  x++;
  delay(50);
}