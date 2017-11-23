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
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(4000);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char recvbuf[BUFFER_SIZE];
    char sendbuf[BUFFER_SIZE]="hello client!";
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn<0)
    {
        perror("connect");
        exit(1);
    }
    else
        printf("连接成功!\n");
    
    int j,i_num=0;
    
    while(1)
    {
        write(conn,sendbuf,sizeof(sendbuf));
        printf("server_send:%s\n",sendbuf);        
        read(conn,recvbuf,sizeof(recvbuf));
        printf("server_recv:%s\n",recvbuf);
        sleep(1);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
