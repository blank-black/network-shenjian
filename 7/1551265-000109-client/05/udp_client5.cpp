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

#define BUFLEN 20  //Max length of buffer
#define TIMEOUT 1
int s1,s2;
char message[BUFLEN];
struct sockaddr_in si_other1,si_other2;
socklen_t slen1,slen2;

void CbSigAlrm(int signo)
{
    sendto(s1, message, sizeof(message),0, (struct sockaddr *) &si_other1, slen1);
    sendto(s2, message, sizeof(message),0, (struct sockaddr *) &si_other2, slen2);
    cout<<"send=";
    int i;
    for(i=0;i<sizeof(message);i++)
        cout<<message[i];
    cout<<endl;
    alarm(TIMEOUT);
}

int main(int argc,char* argv[])
{
    int i,send_size=0,recv1_len,recv2_len;
    char buf[BUFLEN];
    int port1=atoi(argv[2]);
    int port2=atoi(argv[3]);

    if ( (s1=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exit(-1);
    }
    if ( (s2=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exit(-1);
    }
    slen1 = sizeof(si_other1);
    slen2 = sizeof(si_other2);

    memset((char *) &si_other1, 0, sizeof(si_other1));
    memset((char *) &si_other2, 0, sizeof(si_other2));
    si_other1.sin_family = AF_INET;
    si_other1.sin_port = htons(port1);
    si_other1.sin_addr.s_addr = inet_addr(argv[1]); 
    si_other2.sin_family = AF_INET;
    si_other2.sin_port = htons(port2);
    si_other2.sin_addr.s_addr = inet_addr(argv[1]); 

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
        perror("signal");
        return 0;
    }
    int flags = fcntl(s1, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(s1, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；
    flags = fcntl(s2, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(s2, F_SETFL, flags | O_NONBLOCK);   //设置成非阻塞模式；

    for(i=0;i<BUFLEN;i++)
        message[i]=i%10+'0';

    fd_set  fdR;

    alarm(TIMEOUT);

    while(1)
    {
        FD_ZERO(&fdR); 
        FD_SET(s1, &fdR); 

        if(select(s1 + 1, &fdR, NULL,NULL, NULL))
        {
            recv1_len = recvfrom(s1, buf, BUFLEN, 0,(struct sockaddr *) &si_other1, &slen1);
            int flag_zero=0;
            for(i=0;i<BUFLEN;i++)
                if(buf[i])
                    {
                        flag_zero=1;
                        break;
                    }
            if(flag_zero)
            {
                cout<<"recv1_len:"<<recv1_len<<endl;
                for(i=0;i<BUFLEN;i++)
                    cout<<buf[i];
                cout<<endl;
            }
        }

        FD_ZERO(&fdR); 
        FD_SET(s1, &fdR); 
        if(select(s2 + 1, &fdR, NULL,NULL, NULL))
        {
            recv2_len = recvfrom(s2, buf, BUFLEN, 0,(struct sockaddr *) &si_other2, &slen2);
            int flag_zero=0;
            for(i=0;i<BUFLEN;i++)
                if(buf[i])
                    {
                        flag_zero=1;
                        break;
                    }
            if(flag_zero)
            {
                cout<<"recv2_len:"<<recv2_len<<endl;
                for(i=0;i<BUFLEN;i++)
                    cout<<buf[i];
                cout<<endl;
            }
        }

        memset(buf,0,sizeof(buf));
    }
    return 0;
}
