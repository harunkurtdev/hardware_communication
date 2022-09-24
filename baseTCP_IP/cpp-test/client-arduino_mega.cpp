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
    int ret;
    char* ptrChar;

    char bufferX[1024]={0};
    char buffer[1024]={'h','e','l','l','o'};
    
    struct sockaddr_in addr;
    
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("192.168.1.177");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23);

    connect(serverSock,(struct sockaddr *)&addr,sizeof(addr));

    cout<<"Connected to Server"<<endl;
    
	// while (true){
	write(serverSock,buffer,strlen(buffer));

    // while ((ret = read(serverSock, bufferX, sizeof(bufferX)-1)) > 0) {
    // //   bufferX[ret] = 0x00;
    //   std::cout<<"\t"<<bufferX<<"\t"<<"value geldi"<<endl;
    // //  printf("block read: \n<%s>\n", buf);
    // }

    read(serverSock, (char *)&ptrChar, sizeof(ptrChar)-1);
    std::cout<<"\t"<<bufferX<<"\t"<<"value geldi"<<endl;
    
    //char valread = read(serverSock, bufferX, strlen(bufferX));
    //int buf=recv(serverSock, bufferX, strlen(bufferX),0);
    // std::cout<<valread<<"\t"<<"value geldi"<<endl;
    
    // }
    
    close(serverSock);

    std::cout<<"Socket closed."<<endl;
   
}
