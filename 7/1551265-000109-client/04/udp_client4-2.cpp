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
#include <time.h>
#include <signal.h>

using namespace std;

#define BUFLEN 2  //Max length of buffer
#define TIMEOUT 1
int s;
char message[BUFLEN];
struct sockaddr_in si_other;
socklen_t slen;

void CbSigAlrm(int signo)
{
    sendto(s, message, sizeof(message),0, (struct sockaddr *) &si_other, slen);
    cout<<"send=";
    int i;
    for(i=0;i<sizeof(message);i++)
        cout<<message[i];
    cout<<endl;
    alarm(TIMEOUT);
}

int main(int argc,char* argv[])
{
    int i,send_size=0;
    char buf[BUFLEN];
    int port=atoi(argv[2]);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exit(-1);
    }
    slen = sizeof(si_other);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    si_other.sin_addr.s_addr = inet_addr(argv[1]); 

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
        perror("signal");
        return 0;
    }
    int flags = fcntl(s, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(s, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；

    while(1)
    {
        if (sendto(s, message, sizeof(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            exit(-1);
        }
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) >0)
        {
            cout<<"连接成功"<<endl;
            break;
        }
    }
    for(i=0;i<BUFLEN;i++)
        message[i]=i%10+'0';

    alarm(TIMEOUT);
    while(1)
        sleep(1);
    return 0;
}
