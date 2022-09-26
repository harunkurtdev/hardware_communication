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
#define START_FRAMEUno         0xABCDE     	// [-] Start frme definition for reliable serial communication

#define Sensor1 1
#define Sensor2 2

//write for arduino uno
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

// read for arduino uno
typedef struct {
  uint16_t start;
  int16_t id;
  int16_t data;
  int16_t data1;
  uint16_t checksum;
}SerialCommandUno;


SerialCommand Command;
SerialCommand Feedback;

SerialCommandUno CommandUno;
SerialCommandUno FeedbackUno;

uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
byte *p;                                // Pointer declaration for the new received data
byte incomingByte;
byte incomingBytePrev;

uint8_t idxuno = 0;                        // Index for new data pointer
uint16_t bufStartFrameuno;                 // Buffer Start Frame
byte *puno;                                // Pointer declaration for the new received data
byte incomingByteuno;
byte incomingBytePrevuno;


// telnet defaults to port 23
EthernetServer server(23);
bool alreadyConnected = false; // whether or not the client was connected previously

// SerialCommandUno* RecieveUno();
// SerialCommand* RecieveTCP();

void RecieveUno();
void SendUno();
void RecieveTCP(EthernetClient* client);

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
  
  if(server.available()>0){
    EthernetClient client = server.available();
    if (client) {
      if (!alreadyConnected) {
        // clear out the input buffer:
        client.flush();
        Serial.println("We have a new client");
        client.println("Hello, client!");
        alreadyConnected = true;
      }

    RecieveTCP(&client);
    SendUno();
  }else {
    RecieveUno();
  }

  // Serial1.write((uint8_t *)&Command,sizeof(Command));  

    // client.write((uint8_t *)&Command,sizeof(Command));

    // alreadyConnected = false;
    // Serial.println(s);
  }
}

void SendUno(){
  // Feedback.checksum=(uint16_t)Sensor2;
  // Feedback.checksum=(uint16_t)START_FRAMEUno;
  Serial1.write((uint8_t *)&Feedback,sizeof(Feedback));  
}

void RecieveUno(){
  
  if (Serial1.available()>0) {
        incomingByteuno 	  = Serial1.read();                                   // Read the incoming byte
        // Serial.println(incomingByte);
        bufStartFrameuno	= ((uint16_t)(incomingByte) << 8) | incomingBytePrev;       // Construct the start frame
    }
    else {
        return;
    }
    
    // Copy received data
    if (bufStartFrameuno == START_FRAMEUno) {	                    // Initialize if new data is detected
        puno       = (byte *)&CommandUno;
        *puno++    = incomingBytePrevuno;
        *puno++    = incomingByteuno;
        idxuno     = 2;	
    } else if (idxuno >= 2 && idxuno < sizeof(SerialCommandUno)) {  // Save the new received data
        *puno++    = incomingByteuno; 
        idxuno++;
    }	

    if (idxuno == sizeof(SerialCommandUno)) {
        uint16_t checksumuno;
        checksumuno = (uint16_t)Sensor2;
        // Check validity of the new data
        if (CommandUno.start == START_FRAMEUno && checksumuno == CommandUno.checksum) {
            // Copy the new data
            memcpy(&FeedbackUno, &CommandUno, sizeof(SerialCommandUno));

            // Print data to built-in Serial
            Serial.print("0: ");   Serial.print(FeedbackUno.data); Serial.print("\t");
            Serial.print("1: ");   Serial.print(FeedbackUno.data1); Serial.println();
            } else {
          Serial.println("Non-valid data skipped");
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }

    // Update previous states
    incomingBytePrev = incomingByte;

}

void RecieveTCP(EthernetClient* client){
    if (client->available()>0) {
        incomingByte 	  = client->read();                                   // Read the incoming byte
        // Serial.println(incomingByte);
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
    // Serial.println(idx++);
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
            Serial1.write((uint8_t *)&Command,sizeof(Command));
            Serial.println();
            } else {
          Serial.println("Non-valid data skipped");
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }

    // Update previous states
    incomingBytePrev = incomingByte;

}