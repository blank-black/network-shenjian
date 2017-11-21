#include <iostream>
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
using namespace std;
//#define port  8887
#define BUFFER_SIZE 1024
int main(int argc ,char* argv[])
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[3]);
    ///定义sockaddr_in
    struct sockaddr_in servaddr,cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(argv[2]);  ///服务器ip
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(atoi(argv[1]));  ///端口
    cliaddr.sin_addr.s_addr = inet_addr("192.168.2.232");  ///ip


    ///连接服务器，成功返回0，错误返回-1
    if(bind(sock_cli,(struct sockaddr *)&cliaddr,sizeof(cliaddr))==-1)
    {
        perror("bind");
        exit(1);
    }
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    else
	cout<<"连接成功!"<<endl;
    char sendbuf[BUFFER_SIZE];
    char recvbuf[BUFFER_SIZE];
    while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
    {
        if(!recv(sock_cli, recvbuf, sizeof(recvbuf),0))
            break;
        memset(recvbuf, 0, sizeof(recvbuf));
    }

    close(sock_cli);
    return 0;
}
