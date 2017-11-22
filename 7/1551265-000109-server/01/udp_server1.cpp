#include <iostream>
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;
#define BUFLEN 512  //Max length of buffer
  
int main(int argc,char* argv[])
{
    struct sockaddr_in si_me, si_other;
     
    int  i , recv_len;
    char buf[BUFLEN];
    socklen_t slen = sizeof(si_other);
    //create a UDP socket
    int s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == -1)
    {
        exit(-1);
    }
    int port=atoi(argv[1]);
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        exit(-1);
    }
     
    //keep listening for data

    while(1)
    {
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) >0)
        {
            sendto(s, buf, strlen(buf) , 0 , (struct sockaddr *) &si_other, slen);
            cout<<"连接成功"<<endl;
            break;
        }
    }


    while(1)
    {
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            exit(-1);
        }
    }
 
    return 0;
}
