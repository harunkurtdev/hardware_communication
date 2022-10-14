#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>



#include <unistd.h>

#include <string.h> //for memset 

using namespace std;



#define START_FRAME         0xABCD     	// [-] Start frme definition for reliable serial communication
#define START_FRAMEUno         0xABCDE    	// [-] Start frme definition for reliable serial communication

#define Sensor1 1
#define Sensor2 1

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



typedef struct {
  uint16_t start;
  int16_t id;
  int16_t data;
  int16_t data1;
uint16_t checksum;
}SerialCommandUno;

SerialCommand Command;
SerialCommand Feedback;
SerialCommand RecieveCommand;
// SerialCommand CommandServer;
SerialCommandUno CommandServer;

uint8_t idx = 0;                        // Index for new data pointer
uint16_t bufStartFrame;                 // Buffer Start Frame
char *p;                                // Pointer declaration for the new received data
char incomingByte;
char incomingBytePrev;

void RecieveTCP(char incomingByte){
  
    // if (client->available()>0) {
    //     incomingByte 	  = client->read();                                   // Read the incoming byte
        // Serial.println(incomingByte);
    bufStartFrame	= ((uint16_t)(incomingByte) << 8) | (uint8_t)incomingBytePrev;       // Construct the start frame
    // }
    // else {
    //     return;
    // }
    
    // Copy received data

    std::cout<<"Feedback.checksum"<<std::endl;
    std::cout<<incomingByte<<std::endl;
    if (bufStartFrame == START_FRAME) {	                    // Initialize if new data is detected
        p       = (char *)&Command;
        *p++    = incomingBytePrev;
        *p++    = incomingByte;
        idx     = 2;	
    } else if (idx >= 2 && idx < sizeof(SerialCommand)) {  // Save the new received data
        *p++    = incomingByte; 
        idx++;
        
    }	
    std::cout<<idx<<"gelen data"<<std::endl;
    // Serial.println(idx++);
    if (idx == sizeof(SerialCommand)) {
        uint16_t checksum;
        // std::cout<<Sensor1<<std::endl;
        checksum = (uint16_t)Sensor1;
        // Check validity of the new data
        if (Command.start == START_FRAME && checksum == Command.checksum) {
            // Copy the new data
            memcpy(&Feedback, &Command, sizeof(SerialCommand));

            // // Print data to built-in Serial
            std::cout<<"checksum \t "<<Feedback.checksum<<std::endl;
            std::cout<<"data 1 \t"<<Feedback.data1<<std::endl;
            std::cout<<"data 2 \t"<<Feedback.data2<<std::endl;
            std::cout<<"data 3 \t"<<Feedback.data3<<std::endl;
            std::cout<<"data 4 \t"<<Feedback.data4<<std::endl;
            std::cout<<"data 5 \t"<<Feedback.data5<<std::endl;
            std::cout<<"data 6 \t"<<Feedback.data6<<std::endl;
            // Serial.print("0: ");   Serial.print(Feedback.data); Serial.print("\t");
            // Serial.print("1: ");   Serial.print(Feedback.data1); Serial.print("\t");
            // Serial.print("2: ");   Serial.print(Feedback.data2);Serial.print("\t");
            // Serial.print("3: ");   Serial.print(Feedback.data3);Serial.print("\t");
            // Serial.print("4: ");   Serial.print(Feedback.data4);Serial.print("\t");
            // Serial.print("5: ");   Serial.print(Feedback.data5);Serial.print("\t");
            // Serial.print("6: ");   Serial.print(Feedback.data6);
            // Serial1.write((uint8_t *)&Command,sizeof(Command));
            // Serial.println();
            } else {
          // Serial.println("Non-valid data skipped");
          std::cout<<"Non-valid data skipped"<<std::endl;
        }
        idx = 0;    // Reset the index (it prevents to enter in this if condition in the next cycle)
    }

    // Update previous states
    incomingBytePrev = incomingByte;

}

int main(){

    int serverSock{0};
    string s;
    char val;
    int ret;
    char* ptrChar;

    char bufferX[1024]={0};
    char buffer[1024]={'h','e','l','l','o'};
    
    struct sockaddr_in addr;
    
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("192.168.31.177");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23);

    
    cout<<"Connected to Server"<<endl;
    

    Command.start=(uint16_t)START_FRAME;
    Command.id=(int16_t)1;
    
    Command.data1=(int16_t)11;
    Command.data2=(int16_t)2;
    Command.data3=(int16_t)3;
    Command.data4=(int16_t)4;
    Command.data5=(int16_t)5;
    Command.data6=(int16_t)6;
    Command.data7=(int16_t)7;
    Command.checksum=(uint16_t)Sensor1;
    connect(serverSock,(struct sockaddr *)&addr,sizeof(addr));
    int sendData{0};
    char c;
	while (true){
    cout<< "send data for arduino uno : "<<endl;
    // cin>>sendData;

    // cout<< "send data for arduino uno at Sensor 2 : "<<endl;
    // cin>>sendData;
    Command.data=(int16_t)1;
	  write(serverSock,(uint8_t *)&Command,sizeof(SerialCommand));
int i = 0, r = 0;
    while ((r = read(serverSock, (uint8_t *)&c, 1)) > 0 && i++ < 1024){
      // std::cout<<"relax"<<std::endl;
      std::cout<<i<<std::endl;
            RecieveTCP(c);
    
    }
      
    // close(serverSock);
    // read(serverSock, (uint8_t *)&CommandServer,sizeof(CommandServer));
    // cout<<CommandServer.data<<"\t"<<CommandServer.data1<<endl;
    
    }
    // while ((ret = read(serverSock, bufferX, sizeof(bufferX)-1)) > 0) {
    // //   bufferX[ret] = 0x00;
    //   std::cout<<"\t"<<bufferX<<"\t"<<"value geldi"<<endl;
    // //  printf("block read: \n<%s>\n", buf);
    // }

    // read(serverSock, (char *)&ptrChar, sizeof(ptrChar)-1);
    // read(serverSock, (uint8_t *)&CommandServer,sizeof(CommandServer));
    // std::cout<<"\t"<<(uint8_t *)&CommandServer.data1<<"\t"<<"value geldi"<<endl;
    
    //char valread = read(serverSock, bufferX, strlen(bufferX));
    //int buf=recv(serverSock, bufferX, strlen(bufferX),0);
    // std::cout<<valread<<"\t"<<"value geldi"<<endl;
    
    // }
    
    close(serverSock);

    std::cout<<"Socket closed."<<endl;
   
}
