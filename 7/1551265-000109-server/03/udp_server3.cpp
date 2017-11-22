#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <arpa/inet.h>
using namespace std;
#define BUFLEN 20  //Max length of buffer
  
int main(int argc,char* argv[])
{
    struct sockaddr_in si_me, si_other;
     
    int  i , recv_len;
    char buf[BUFLEN];
    memset(buf,0,sizeof(buf));
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
            sendto(s, buf, sizeof(buf) , 0 , (struct sockaddr *) &si_other, slen);
            cout<<"连接成功"<<endl;
            break;
        }
    }
    memset(buf,0,sizeof(buf));

    int recv_min_size = BUFLEN;
    cout<<setsockopt(s,SOL_SOCKET,SO_RCVLOWAT,(void *)&recv_min_size,sizeof(int))<<endl;

    fd_set  fdR;
    FD_ZERO(&fdR); 
    FD_SET(s, &fdR); 
    select(s + 1, &fdR, NULL,NULL, NULL);
    recv_len = recv(s, buf, BUFLEN, 0);
    cout<<"recv_len:"<<recv_len<<endl;
    cout<<"recv:";
    for(i=0;i<BUFLEN;i++)
        cout<<buf[i];
    cout<<endl;
    return 0;
}
