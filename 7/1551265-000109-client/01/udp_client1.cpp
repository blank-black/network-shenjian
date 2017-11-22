#include <iostream>
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <sys/socket.h>
using namespace std;

#define BUFLEN 512  //Max length of buffer
  
int main(int argc,char* argv[])
{
    struct sockaddr_in si_other;
    int s, i;
    char buf[BUFLEN];
    char message[BUFLEN];
    int port=atoi(argv[2]);

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        exit(-1);
    }
    socklen_t slen = sizeof(si_other);
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    si_other.sin_addr.s_addr = inet_addr(argv[1]); 
    while(1)
    {
        if (sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            exit(-1);
        }
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) >0)
        {
            cout<<"连接成功"<<endl;
            break;
        }
    }
    while(1)
    {
        if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
        {
            exit(-1);
        }
         
    }
 
    return 0;
}
