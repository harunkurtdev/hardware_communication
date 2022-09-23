#include <Arduino.h>


// #define DEBUG_RX
#define Sensor1 1
#define Sensor2 2

#define START_FRAME         0xABCD     	// [-] Start frme definition for reliable serial communication


//Serail communaction for arduino uno 
#include <SoftwareSerial.h>
SoftwareSerial CommandSerialCom(2,3);        // RX, TX


//my packet for serialcommand
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
SerialCommand Feedback;

uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;


void Receive()
{
    // Check for new data availability in the Serial buffer
    if (CommandSerialCom.available()) {
        incomingByte 	  = CommandSerialCom.read();                                   // Read the incoming byte
        bufStartFrame	= ((uint16_t)(incomingByte) << 8) | incomingBytePrev;       // Construct the start frame
    }
    else {
        return;
    }

  // If DEBUG_RX is defined print all incoming bytes
  // #ifdef DEBUG_RX
  //       Serial.print(incomingByte);
  //       return;
  //   #endif

    // Copy received data
    if (bufStartFrame == START_FRAME) {	                    // Initialize if new data is detected
        p       = (byte *)&Command;
        *p++    = incomingBytePrev;
        *p++    = incomingByte;
        idx     = 2;	
    } else if (idx >= 2 && idx < sizeof(SerialCommand)) {  // Save the new received data
        *p++    = incomingByte; 
        idx++;
    }	
    
    // Check if we reached the end of the package
    if (idx == sizeof(SerialCommand)) {
        uint16_t checksum;
        checksum = (uint16_t)Sensor1;
        // Check validity of the new data
        if (Command.start == START_FRAME && checksum == Command.checksum) {
            // Copy the new data
            memcpy(&Feedback, &Command, sizeof(SerialCommand));

            // Print data to built-in Serial
            Serial.print("0: ");   Serial.print(Feedback.data); Serial.print("\t");
            Serial.print("1: ");   Serial.print(Feedback.data1); Serial.print("\t");
            Serial.print("2: ");   Serial.print(Feedback.data2);Serial.print("\t");
            Serial.print("3: ");   Serial.print(Feedback.data3);Serial.print("\t");
            Serial.print("4: ");   Serial.print(Feedback.data4);Serial.print("\t");
            Serial.print("5: ");   Serial.print(Feedback.data5);Serial.print("\t");
            Serial.print("6: ");   Serial.print(Feedback.data6);
            Serial.println();
            } else {
          Serial.println("Non-valid data skipped");
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }

    // Update previous states
    incomingBytePrev = incomingByte;
}


void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT); // for control motor and etc...
  Serial.begin(9600);
  CommandSerialCom.begin(9600); //software serail
}




void loop() {
  // put your main code here, to run repeatedly:


  //if you send at master arduino mega you remove commit line 
  //and change value other Command nanem 
  // Command.id=(uint16_t)1;
  // Command.data=(uint16_t)1;
  // Command.checksum=(uint16_t)Sensor1;

  // Serial.write((uint8_t *)&Command,sizeof(Command));Serial.println();


  Receive(); // recieve data for sensor or control from master and etc

  switch(Command.checksum){
    case Sensor1:
      digitalWrite(13,Command.data==1?HIGH:LOW);
      break;
    case Sensor2:
      digitalWrite(10,Command.data==1?HIGH:LOW);
      break;
    default:
      break;
  }

}