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

    int flags = fcntl(s, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(s, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；

     
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

    fd_set  fdR; 

    while(1)
    {
        FD_ZERO(&fdR); 
        FD_SET(s, &fdR); 
        select(s + 1, &fdR, NULL,NULL, NULL);//非阻塞recv
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
    }
 
    return 0;
}
