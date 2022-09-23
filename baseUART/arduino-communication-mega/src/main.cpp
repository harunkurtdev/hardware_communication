#include <Arduino.h>

#define START_FRAME         0xABCD     	// [-] Start frme definition for reliable serial communication

#define Sensor1 1

typedef struct {
  uint16_t start;
  int16_t id;
  int16_t data;
  int16_t data1;
  int16_t data2;
  int16_t data3;
  int16_t data4;
  int16_t data5;
  int16_t data6;
  int16_t data7;
  uint16_t checksum;
}SerialCommand;

SerialCommand Command;

void setup() {
  // put your setup code here, to run once:

  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
      int a=Serial.read();
      Command.start=(uint16_t)START_FRAME;
      Command.id=(int16_t)1;
      Command.data=(int16_t)a;
      Command.data1=(int16_t)11;
      Command.data2=(int16_t)2;
      Command.data3=(int16_t)3;
      Command.data4=(int16_t)4;
      Command.data5=(int16_t)5;
      Command.data6=(int16_t)6;
      Command.data7=(int16_t)7;
      Command.checksum=(uint16_t)Sensor1;

      Serial1.write((uint8_t *)&Command,sizeof(Command));Serial.println();
      
  }