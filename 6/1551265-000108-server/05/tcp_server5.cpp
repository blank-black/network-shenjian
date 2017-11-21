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
#include <time.h>
#include <signal.h>
using namespace std;
#define QUEUE   20
#define BUFFER_SIZE 100
#define SEND_SIZE 10
#define TIMEOUT 1
int num,i;
int max_fd=0,client_fds[QUEUE]={0};
char buffer[BUFFER_SIZE],send_buf[SEND_SIZE];

void CbSigAlrm(int signo)
{
    int j,flag=0;
    for(j=0;j<QUEUE;j++)
        if(client_fds[j])
            flag+=send(client_fds[j], send_buf, sizeof(send_buf),MSG_DONTWAIT);
    if(flag)
    {
        cout<<"send=";
        for(j=0;j<sizeof(send_buf);j++)
            cout<<send_buf[j];
        cout<<endl;
    }
    alarm(TIMEOUT);
}

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

    if(signal(SIGALRM,CbSigAlrm)==SIG_ERR)
    {
        perror("signal");
        return 0;
    }
    //关闭标准输出的行缓存模式
    setbuf(stdout,NULL);

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
    for(num=0;num<SEND_SIZE;num++)
        send_buf[num]=num+'0';
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    fd_set fdR; 
    alarm(TIMEOUT);

    while(1)
    {
        FD_ZERO(&fdR);
        FD_SET(server_sockfd, &fdR); 
        if(max_fd<server_sockfd)
            max_fd=server_sockfd; 
        for(i=0;i<QUEUE;i++)
            if(client_fds[i])
            {
                FD_SET(client_fds[i], &fdR);
                if(max_fd<client_fds[i])
                    max_fd=client_fds[i];
            }
        ///成功返回非负描述字，出错返回-1
        int ret=select(max_fd + 1, &fdR, NULL,NULL, 0); //非阻塞connect
        if(ret>0)
        {
            if(FD_ISSET(server_sockfd, &fdR))  
            {  
            //有新的连接请求  
                struct sockaddr_in client_address;  
                socklen_t address_len;
                int conn=0;  
                conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length); 
                if(conn > 0)  
                    for(i = 0; i < QUEUE; i++)  
                        if(!client_fds[i])  
                        {
                            client_fds[i] = conn; 
                            cout<<"连接成功!"<<endl;
                            break;
                        }
                int flags2 = fcntl(client_fds[i], F_GETFL, 0);        //获取文件的flags2值。
                fcntl(client_fds[i], F_SETFL, flags2 | O_NONBLOCK);   //设置成非阻塞模式；
            }

            for(i=0;i<QUEUE;i++)
                if(client_fds[i])
                {
                    if(FD_ISSET(client_fds[i], &fdR))
                    {
                        memset(buffer,0,sizeof(buffer));
                        int len=recv(client_fds[i], buffer, sizeof(buffer),0);
                        if(len>0)
                        {
                            cout<<"recv=";
                            int k;
                            for(k=0;k<len;k++)
                                cout<<buffer[k];
                            cout<<endl;
                        }
                        else if(!len)
                        {
                            FD_CLR(client_fds[i], &fdR);  
                            client_fds[i] = 0;  
                        }
                    }
                }
        }
    }
    close(server_sockfd);
    return 0;
}
