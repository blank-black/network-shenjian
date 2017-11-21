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

int main(int argc ,char* argv[])
{
    ///定义sockfd
    int sock_cli = socket(AF_INET,SOCK_STREAM, 0);
    int port=atoi(argv[2]);
    int read_num=atoi(argv[3]);
    int write_num=atoi(argv[4]);

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
    char buf_r[read_num];
    char buf_w[write_num];
    int j,i_num=0;

    while (1)
    {
        memset(buf_r,0,sizeof(buf_r));
        read(sock_cli,buf_r,sizeof(buf_r));
        for(j=0;j<write_num;j++)
            buf_w[j]=rand()%10+'0';
        i_num+=sizeof(buf_w);
        write(sock_cli,buf_w,sizeof(buf_w));
        cout<<"发送字节数:"<<i_num<<endl;
        if(strcmp(buf_r,"exit\n")==0)
            break;    
    }

    close(sock_cli);
    return 0;
}
