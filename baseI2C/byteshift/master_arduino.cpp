int pot=0;
#define potpin A0
#include <Wire.h>

union byteint
{
    byte b[sizeof(int)];
    int i;
};

void setup()
{
  Wire.begin(); 
  Serial.begin(9600);
}

void loop()
{  
  
  byteint bi;
  //bi.i = 65535+1+1; 
  // max value 65535 for our arduino
  // min value is 0 
  // int is unsigned int value
  
  pot= analogRead(potpin); // read pot
  
  bi.i=pot; //val write
  Serial.println(pot);
  Wire.beginTransmission(9);
  Serial.print(bi.b[0]);
  Serial.print("-");
  Serial.print(bi.b[1]);
  
  Wire.write(bi.b[0]); //send first byte
  Wire.write(bi.b[1]); //send second byte
  Wire.endTransmission();
  Serial.println();

//for(int i = 0; i<4;i++)
    //destination[i] = bi.b[i];
  	//Serial.println(bi.b[i]);
  
 
 //Serial.print(bi.b[2]);
 //Serial.print(bi.b[3]);
 
}