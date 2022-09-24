#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>



#include <unistd.h>

#include <string.h> //for memset 

using namespace std;

int main(){

    int serverSock{0};
	string s;
	char val;
	
	//val="val\n";
	
    //strcpy(s,"val\n");
    
    struct sockaddr_in addr;
    
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

   
    addr.sin_addr.s_addr = inet_addr("192.168.1.177");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23);

    connect(serverSock,(struct sockaddr *)&addr,sizeof(addr));

    cout<<"Connected to Server"<<endl;
    char buffer[1024]={'h','e','l','l','o','\0'};
    
	//while (true){
	write(serverSock,buffer,strlen(buffer));
	//write(serverSock,(char* )&s,sizeof(s));
    char bufferX[1024]={0};
    // char valread = read(serverSock, bufferX, strlen(bufferX));
    std::cout<<recv(serverSock, bufferX, strlen(bufferX),0)<<bufferX<<"value geldi"<<endl;
    //	}
    
    close(serverSock);

    std::cout<<"Socket closed."<<endl;
   
}
