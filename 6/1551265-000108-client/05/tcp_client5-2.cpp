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
int sock_cli1,sock_cli2;

void CbSigAlrm(int signo)
{
    send(sock_cli1, send_buf, sizeof(send_buf),MSG_DONTWAIT);
    send(sock_cli2, send_buf, sizeof(send_buf),MSG_DONTWAIT);
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
    sock_cli1 = socket(AF_INET,SOCK_STREAM, 0);
    sock_cli2 = socket(AF_INET,SOCK_STREAM, 0);
    int port1=atoi(argv[2]);
    int port2=atoi(argv[3]);

    ///定义sockaddr_in
    struct sockaddr_in servaddr1,servaddr2;
    memset(&servaddr1, 0, sizeof(servaddr1));
    servaddr1.sin_family = AF_INET;
    servaddr1.sin_port = htons(port1);  ///服务器端口
    servaddr1.sin_addr.s_addr = inet_addr(argv[1]);  ///服务器ip
    memset(&servaddr2, 0, sizeof(servaddr2));
    servaddr2.sin_family = AF_INET;
    servaddr2.sin_port = htons(port2);  ///服务器端口
    servaddr2.sin_addr.s_addr = inet_addr(argv[1]);  ///服务器ip

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
        perror("signal");
        return 0;
    }
    //关闭标准输出的行缓存模式
    setbuf(stdout,NULL);

    int flags1 = fcntl(sock_cli1, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(sock_cli1, F_SETFL, flags1 | O_NONBLOCK);   //设置成非阻塞模式；
    int flags2 = fcntl(sock_cli2, F_GETFL, 0);                        //获取文件的flags1值。
    fcntl(sock_cli2, F_SETFL, flags1 | O_NONBLOCK);   //设置成非阻塞模式；

    fd_set  fdR1,fdR2; 
    FD_ZERO(&fdR1); 
    FD_SET(sock_cli1, &fdR1); 
    FD_ZERO(&fdR2); 
    FD_SET(sock_cli2, &fdR2); 

    switch (select(sock_cli1 + 1, &fdR1, NULL,NULL, 0)) //非阻塞connect
    {
        case -1:
            perror("select");
            break;/* 这说明select函数出错 */
        case 0:
            sleep(1);
            printf("超时\n");
            break; /* 说明在设定的时间内，socket的状态没有发生变化 */
        default:
            connect(sock_cli1, (struct sockaddr *)&servaddr1, sizeof(servaddr1));
    }
    cout<<"1连接成功!"<<endl;

    switch (select(sock_cli2 + 1, &fdR2, NULL,NULL, 0)) //非阻塞connect
    {
        case -1:
            perror("select");
            break;/* 这说明select函数出错 */
        case 0:
            sleep(1);
            printf("超时\n");
            break; /* 说明在设定的时间内，socket的状态没有发生变化 */
        default:
            connect(sock_cli2, (struct sockaddr *)&servaddr2, sizeof(servaddr2));            
    }
    cout<<"2连接成功!"<<endl;

    int num;
    for(num=0;num<SEND_SIZE;num++)
        send_buf[num]=num+'0';
    alarm(TIMEOUT);

    while(1)
    {
        FD_ZERO(&fdR1); 
        FD_SET(sock_cli1, &fdR1); 
        switch (select(sock_cli1 + 1, &fdR1, NULL,NULL,0)) //非阻塞recv
        {
            case 0:
                break; /* 说明在设定的时间内，socket的状态没有发生变化 */
            default:
                memset(buffer,0,sizeof(buffer));
                int len=recv(sock_cli1, buffer, sizeof(buffer),MSG_DONTWAIT);
                if(len>0)
                {
                    cout<<"1recv=";
                    for(num=0;num<len;num++)
                        cout<<buffer[num];
                    cout<<endl;
                }
        }
        FD_ZERO(&fdR2); 
        FD_SET(sock_cli2, &fdR2); 
        switch (select(sock_cli2 + 1, &fdR2, NULL,NULL,0)) //非阻塞recv
        {
            case 0:
                break; /* 说明在设定的时间内，socket的状态没有发生变化 */
            default:
                memset(buffer,0,sizeof(buffer));
                int len=recv(sock_cli2, buffer, sizeof(buffer),MSG_DONTWAIT);
                if(len>0)
                {
                    cout<<"2recv=";
                    for(num=0;num<len;num++)
                        cout<<buffer[num];
                    cout<<endl;
                }
        }

    }
    close(sock_cli1);
    close(sock_cli2);
    return 0;
}

