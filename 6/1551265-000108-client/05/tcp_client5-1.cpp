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
#include <time.h>
#include <signal.h>

using namespace std;
#define BUFFER_SIZE 100
#define SEND_SIZE 15
#define TIMEOUT 3

char send_buf[SEND_SIZE],buffer[BUFFER_SIZE];
int sock_cli;

void CbSigAlrm(int signo)
{
    send(sock_cli, send_buf, sizeof(send_buf),MSG_DONTWAIT);
    cout<<"send=";
    int i;
    for(i=0;i<sizeof(send_buf);i++)
        cout<<send_buf[i];
    cout<<endl;
    alarm(TIMEOUT);
}


int main(int argc ,char* argv[])
{
    ///定义sockfd
    sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    ///定义sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);  ///服务器端口
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);  ///服务器ip

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
    perror("signal");
    return 0;
    }
    //关闭标准输出的行缓存模式
    setbuf(stdout,NULL);

    int flags1 = fcntl(sock_cli, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(sock_cli, F_SETFL, flags1 | O_NONBLOCK);   //设置成非阻塞模式；

    fd_set  fdR; 
    FD_ZERO(&fdR); 
    FD_SET(sock_cli, &fdR); 
    switch (select(sock_cli + 1, &fdR, NULL,NULL, 0)) //非阻塞connect
    {
        case -1:
            perror("select");
            break;/* 这说明select函数出错 */
        case 0:
            sleep(1);
            printf("超时\n");
            break; /* 说明在设定的时间内，socket的状态没有发生变化 */
        default:
            connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
    cout<<"连接成功!"<<endl;

    int num;
    for(num=0;num<SEND_SIZE;num++)
        send_buf[num]=num+'0';
    alarm(TIMEOUT);

    while(1)
    {
        FD_ZERO(&fdR); 
        FD_SET(sock_cli, &fdR); 
        switch (select(sock_cli + 1, &fdR, NULL,NULL,0)) //非阻塞recv
        {
            case 0:
                break; /* 说明在设定的时间内，socket的状态没有发生变化 */
            default:
                memset(buffer,0,sizeof(buffer));
                int len=recv(sock_cli, buffer, sizeof(buffer),MSG_DONTWAIT);
                if(len>0)
                {
                    cout<<"recv=";
                    for(num=0;num<len;num++)
                        cout<<buffer[num];
                    cout<<endl;
                }
       }
    }
    close(sock_cli);
    return 0;
}

