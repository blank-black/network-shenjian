#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <net/if.h>
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
#define BUFFER_SIZE 1024

int get_local_ip() 
{
    char ip[20][20];
    int fd, intrface, retn = 0,i=0;
    struct ifreq buf[INET_ADDRSTRLEN];
    struct ifconf ifc;
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) 
    {
        ifc.ifc_len = sizeof(buf);
        // caddr_t,Linux内核源码里定义的：typedef void *caddr_t；
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) 
        {
            intrface = ifc.ifc_len / sizeof(struct ifreq);
            for (i=0;intrface-- > 0;i++) 
            {
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
                    strcpy(ip[i],(inet_ntoa(((struct sockaddr_in *) (&buf[intrface].ifr_addr))->sin_addr)));
                    printf("IP:%s\n", ip[i]);
                }
            }
        }
        close(fd);
        return i;
    }
}

int main(int argc,char* argv[]) 
{
    ///定义sockfd
    int server_sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int enable = 1;
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");
    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    int port = atoi(argv[2]);

    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(port);
    server_sockaddr.sin_addr.s_addr = inet_addr(argv[1]);
    get_local_ip();

    ///bind，成功返回0，出错返回-1
    if (bind(server_sockfd, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1) 
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if (listen(server_sockfd, QUEUE) == -1) 
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    ///成功返回非负描述字，出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr *) &client_addr, &length);
    if (conn < 0) 
    {
        perror("connect");
        exit(1);
    } else
        cout << "连接成功!" << endl;

    struct sockaddr_in c;
    socklen_t cLen = sizeof(c);
    getpeername(conn, (struct sockaddr *) &c, &cLen);
    cout << "client IP:" << inet_ntoa(c.sin_addr) << "  port:" << ntohs(c.sin_port) << endl;

    while (1) 
    {
        memset(buffer, 0, sizeof(buffer));
        if(!recv(conn, buffer, sizeof(buffer), 0))
            break;
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
