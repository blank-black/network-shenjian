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
#define QUEUE   20
#define BUFFER_SIZE 20
int main(int argc,char* argv[])
{
    ///定义sockfd
    int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");
    ///定义sockaddr_in
    int port=atoi(argv[1]);
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);


    int flags1 = fcntl(server_sockfd, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(server_sockfd, F_SETFL, flags1 | O_NONBLOCK);   //设置成非阻塞模式；

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
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
    int conn;
    fd_set  fdR; 
    FD_ZERO(&fdR); 
    FD_SET(server_sockfd, &fdR); 
    switch (select(server_sockfd + 1, &fdR, NULL,NULL, NULL)) //非阻塞connect
    {
        case -1:
            perror("select");
            break;/* 这说明select函数出错 */
        case 0:
            sleep(1);
            printf("超时\n");
            break; /* 说明在设定的时间内，socket的状态没有发生变化 */
        default:
            conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    }
    cout<<"连接成功!"<<endl;
    
    // getchar();
    
    int flags2 = fcntl(conn, F_GETFL, 0);                        //获取文件的flags2值。
    fcntl(conn, F_SETFL, flags2 | O_NONBLOCK);   //设置成非阻塞模式；

    
    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        read(conn, buffer, sizeof(buffer));
        sleep(1);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
