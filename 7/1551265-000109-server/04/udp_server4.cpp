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
            sendto(s, buf, sizeof(buf) , 0 , (struct sockaddr *) &si_other, slen);
            cout<<"连接成功"<<endl;
            break;
        }
    }
    memset(buf,0,sizeof(buf));

    fd_set  fdR;
    FD_ZERO(&fdR); 
    FD_SET(s, &fdR); 
    while(1)
    {
        if(select(s + 1, &fdR, NULL,NULL, NULL))
        {
            recv_len = recvfrom(s, buf, BUFLEN, 0,(struct sockaddr *) &si_other, &slen);
            int flag_zero=0;
            for(i=0;i<BUFLEN;i++)
                if(buf[i])
                    {
                        flag_zero=1;
                        break;
                    }
            if(flag_zero)
            {
                cout<<"recv_len:"<<recv_len<<endl;
                for(i=0;i<BUFLEN;i++)
                    cout<<buf[i];
                cout<<endl;
                break;
            }
        }
    }
 
    return 0;
}
