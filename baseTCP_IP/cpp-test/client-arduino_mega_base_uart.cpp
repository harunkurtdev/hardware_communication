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
#define START_FRAME         0xABCDE    	// [-] Start frme definition for reliable serial communication

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
// SerialCommand CommandServer;
SerialCommandUno CommandServer;

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

    addr.sin_addr.s_addr = inet_addr("192.168.1.177");
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
	while (true){
    cout<< "send data for arduino uno : "<<endl;
    cin>>sendData;

    cout<< "send data for arduino uno at Sensor 2 : "<<endl;
    cin>>sendData;
    Command.data=(int16_t)sendData;
	write(serverSock,(uint8_t *)&Command,sizeof(Command));
    // close(serverSock);
    read(serverSock, (uint8_t *)&CommandServer,sizeof(CommandServer));
    cout<<CommandServer.data<<"\t"<<CommandServer.data1<<endl;
    
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
