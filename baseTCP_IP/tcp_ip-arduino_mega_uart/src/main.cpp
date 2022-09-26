#include <Arduino.h>

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);
IPAddress myDns(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);


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
SerialCommand Feedback;

uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;


// telnet defaults to port 23
EthernetServer server(23);
bool alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  // initialize the Ethernet device
  Ethernet.begin(mac, ip, myDns, gateway, subnet);
  // our serial communication
  Serial1.begin(9600);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   
   while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  // start listening for clients
  server.begin();

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  String s;
  // when the client sends the first byte, say hello:
   Command.start=(uint16_t)START_FRAME;
    Command.id=(int16_t)1;
    Command.data=(int16_t)2;
    Command.data1=(int16_t)11;
    Command.data2=(int16_t)2;
    Command.data3=(int16_t)3;
    Command.data4=(int16_t)4;
    Command.data5=(int16_t)5;
    Command.data6=(int16_t)6;
    Command.data7=(int16_t)7;
    Command.checksum=(uint16_t)Sensor1;

     
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    // if (client.available() > 0) {
    //   // read the bytes incoming from the client:
    //   char thisChar = client.read();
    //   if (thisChar=='\0')
    //     s+='\n';
    //   // echo the bytes back to the client:
    //   //server.write(thisChar);
    //   client.write("thisChar");
    //   // echo the bytes to the server as well:
    //   Serial.write(thisChar);
    // }

    if (client.available()>0) {
        incomingByte 	  = client.read();                                   // Read the incoming byte
        Serial.println(incomingByte);
        bufStartFrame	= ((uint16_t)(incomingByte) << 8) | incomingBytePrev;       // Construct the start frame
    }
    else {
        return;
    }
    
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
    
    // Serial1.write((uint8_t *)&Command,sizeof(Command));Serial.println();
    
    // alreadyConnected = false;
    // Serial.println(s);
  }
}