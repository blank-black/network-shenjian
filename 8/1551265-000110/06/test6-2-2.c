#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#define BUFFER_SIZE 1024

int main(int argc ,char* argv[])
{
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    ///定义sockfd
    int sock_cli = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    socklen_t slen = sizeof(servaddr);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(4000);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  ///服务器ip
    char recvbuf[BUFFER_SIZE];
    char sendbuf[BUFFER_SIZE]="hello server!";
    int j,i_num=0;

    while (1)
    {
        sendto(sock_cli, sendbuf, BUFFER_SIZE, 0, (struct sockaddr *) &servaddr, slen);
        printf("client_send:%s\n",sendbuf);        
        recvfrom(sock_cli, recvbuf, BUFFER_SIZE, 0, (struct sockaddr *) &servaddr, &slen);
        printf("client_recv:%s\n",recvbuf);
        sleep(1);
    }
    close(sock_cli);
    return 0;
}
