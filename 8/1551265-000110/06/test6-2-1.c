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
#define QUEUE   20
#define BUFFER_SIZE 1024
int main(int argc,char* argv[])
{
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    ///定义sockfd
    int server_sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr,client_addr;
    socklen_t slen = sizeof(client_addr);
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(4000);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    char recvbuf[BUFFER_SIZE];
    char sendbuf[BUFFER_SIZE]="hello client!";
    socklen_t length = sizeof(client_addr);
    int j,i_num=0;
    
    while(1)
    {
        sendto(server_sockfd, sendbuf, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, slen);
        printf("server_send:%s\n",sendbuf);        
        recvfrom(server_sockfd, recvbuf, BUFFER_SIZE, 0, (struct sockaddr *) &client_addr, &slen);
        printf("server_recv:%s\n",recvbuf);
        sleep(1);
    }
    close(server_sockfd);
    return 0;
}
