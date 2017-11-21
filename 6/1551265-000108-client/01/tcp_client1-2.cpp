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
#define BUFFER_SIZE 1024


int main(int argc ,char* argv[])
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  ///服务器ip



    ///连接服务器，成功返回0，错误返回-1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("connect");
        exit(1);
    }
    else
	cout<<"连接成功!"<<endl;

    int flags1 = fcntl(sock_cli, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(sock_cli, F_SETFL, flags1 | O_NONBLOCK);   //设置成非阻塞模式；

    char recvbuf[BUFFER_SIZE];
    int flag;
    fd_set  fdR; 
        FD_ZERO(&fdR); 
        FD_SET(sock_cli, &fdR); 
        switch (select(sock_cli + 1, &fdR, NULL,NULL, NULL)) 
        {
            case -1:
                perror("select");
                break;/* 这说明select函数出错 */
            case 0:
                sleep(1);
                printf("超时\n");
                break; /* 说明在设定的时间内，socket的状态没有发生变化 */
            default:
            memset(recvbuf,0,sizeof(recvbuf));
            flag=recv(sock_cli, recvbuf, sizeof(recvbuf),MSG_DONTWAIT);
        }
    close(sock_cli);
    return 0;
}
