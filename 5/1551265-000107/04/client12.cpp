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
#define BUFFER_SIZE 100
#define per_size 2

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
    char recvbuf[BUFFER_SIZE],buf[per_size];
    int len,i;
    while (1)
    {
        for(i=0;i<99;i++)
        {
            buf[0]=i/10+'0',buf[1]=i%10+'0';
            write(sock_cli, buf, sizeof(buf)); ///发送
            cout<<"发送:"<<buf[0]<<buf[1]<<endl;
            sleep(1);
            recv(sock_cli, recvbuf, sizeof(recvbuf),MSG_DONTWAIT); ///接收
    	    if(strcmp(recvbuf,"exit\n")==0)
                break;        
    	    fputs(recvbuf, stdout);
            memset(recvbuf, 0, sizeof(recvbuf));
        }
        memset(buf, 0, sizeof(buf));
    }

    close(sock_cli);
    return 0;
}
