#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/prctl.h>
#include <signal.h>
#include <stdarg.h>
#include <fstream>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/wait.h>
using namespace std;
#define BUFSIZE 100000
#define QUEUE 1000

void func_waitpid(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0);
}

int main(int argc ,char* argv[])
{
	int pidd = fork();
	if (pidd > 0)
		return 0;
	signal(SIGCHLD,&func_waitpid);
	int num = argc - 1, port = -1, if_tcp = 1, if_block = 0, if_fork = 0;
	int i, j, k, n;
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	for (n = 1; n <= num; n++)
	{
		if (!strcmp(argv[n], "-ip"))
		{
			n++;
			server_sockaddr.sin_addr.s_addr = inet_addr(argv[n]);
		}
		else if (!strcmp(argv[n], "-port"))
		{
			n++;
			port = atoi(argv[n]);
			server_sockaddr.sin_port = htons(port);
		}
		else if (!strcmp(argv[n], "-p"))
		{
			n++;
			if (!strcmp(argv[n], "udp"))
				if_tcp = 0;
		}
		else if (!strcmp(argv[n], "-nonblock") || !strcmp(argv[n], "-nofork"));
		else if (!strcmp(argv[n], "-block"))
			if_block = 1;
		else if (!strcmp(argv[n], "-fork"))
			if_fork = 1;
		else
			return -1;
	}
	if (!if_fork && if_block)
		if_block = 0;
	if (port == -1)
		return -1;

	char buf_send[10][BUFSIZE], buf_recv[BUFSIZE];
	buf_send[0][0] = 'S', buf_send[0][1] = 't', buf_send[0][2] = 'u', buf_send[0][3] = 'N', buf_send[0][4] = 'o';
	buf_send[1][0] = 'p', buf_send[1][1] = 'i', buf_send[1][2] = 'd';
	strcpy(buf_send[2], "TIME");
	int randint = rand() % 99999 + 1;
	buf_send[3][0] = 's', buf_send[3][1] = 't', buf_send[3][2] = 'r';
	for (i = 0; i < 5; i++)
		buf_send[3][i + 3] = (int(randint / pow(10, 4 - i))) % 10 + '0';
	buf_send[3][8] = '\0';
	buf_send[4][0] = 'e', buf_send[4][1] = 'n', buf_send[4][2] = 'd';
	int stuno[QUEUE] = {0};
	int pid[QUEUE] = {0};
	char TIME[QUEUE][19];
	char **random = new char *[QUEUE];
	for (int i = 0; i < QUEUE; i++)
		random[i] = new char[99999];

	if (!if_fork)
	{
		int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		int max_fd = 0, client_fds[QUEUE] = {0}, client_status[QUEUE] = {0};
		int enable = 1;
		if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			perror("setsockopt(SO_REUSEADDR) failed");

		int flags1 = fcntl(server_sockfd, F_GETFL, 0);
		fcntl(server_sockfd, F_SETFL, flags1 | O_NONBLOCK);

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

		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		fd_set fdR;

		while (1)
		{
			FD_ZERO(&fdR);
			FD_SET(server_sockfd, &fdR);
			if (max_fd < server_sockfd)
				max_fd = server_sockfd;
			for (i = 0; i < QUEUE; i++)
				if (client_fds[i])
				{
					FD_SET(client_fds[i], &fdR);
					if (max_fd < client_fds[i])
						max_fd = client_fds[i];
				}
			int ret = select(max_fd + 1, &fdR, NULL, NULL, 0);
			if (ret > 0)
			{
				if (FD_ISSET(server_sockfd, &fdR))
				{
					struct sockaddr_in client_address;
					socklen_t address_len;
					int conn = 0;
					conn = accept(server_sockfd, (struct sockaddr *) &client_addr, &length);
					if (conn > 0)
						for (i = 0; i < QUEUE; i++)
							if (!client_fds[i])
							{
								client_fds[i] = conn;
								send(client_fds[i], buf_send[0], 5, 0);
								client_status[i]++;
								break;
							}
					int flags2 = fcntl(client_fds[i], F_GETFL, 0);
					fcntl(client_fds[i], F_SETFL, flags2 | O_NONBLOCK);
				}

				for (i = 0; i < QUEUE; i++)
					if (client_fds[i])
					{
						if (FD_ISSET(client_fds[i], &fdR))
						{
							memset(buf_recv, 0, sizeof(buf_recv));
							if (client_status[i] == 1)
							{
								recv(client_fds[i], &stuno[i], 4, 0);
								stuno[i] = ntohl(stuno[i]);
								send(client_fds[i], buf_send[client_status[i]], 3, 0);
								client_status[i]++;
							}
							else if (client_status[i] == 2)
							{
								recv(client_fds[i], &pid[i], 4, 0);
								pid[i] = ntohl(pid[i]);
								send(client_fds[i], buf_send[client_status[i]], 5, 0);
								client_status[i]++;
							}
							else if (client_status[i] == 3)
							{
								int len = recv(client_fds[i], buf_recv, sizeof(buf_recv), 0);
								if (len != 19)
								{
									close(client_fds[i]);
									FD_CLR(client_fds[i], &fdR);
									client_fds[i] = 0;
								}
								else
								{
									for (j = 0; j < 19; j++)
										TIME[i][j] = buf_recv[j];
									send(client_fds[i], buf_send[client_status[i]], 9, 0);
									client_status[i]++;
								}
							}
							else if (client_status[i] == 4)
							{
								recv(client_fds[i], buf_recv, sizeof(buf_recv), 0);
								for (j = 0; j < randint; j++)
									random[i][j] = buf_recv[j];
								send(client_fds[i], buf_send[client_status[i]], 3, 0);
								client_status[i]++;
							}
							else if (client_status[i] == 5)
							{
								if (recv(client_fds[i], buf_recv, sizeof(buf_recv), 0) <= 0)
								{
									FD_CLR(client_fds[i], &fdR);
									client_fds[i] = 0;
									if (client_status[i] == 5)
									{
										char filename[100];
										sprintf(filename, "1551265.%d.pid.txt", pid[i]);
										ofstream fout(filename);
										if (fout.is_open())
										{
											fout << stuno[i] << endl << pid[i] << endl;
											for (j = 0; j < 19; j++)
												fout << TIME[i][j];
											fout << endl;
											for (j = 0; j < randint; j++)
												fout << random[i][j];
										}
										fout.close();
									}

								}

							}
						}
					}
			}
		}
	}


	else if (if_fork && if_block)
	{
		int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		int client_status[QUEUE] = {0};
		int enable = 1;
		if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			perror("setsockopt(SO_REUSEADDR) failed");

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

		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		for (i = 0; i < QUEUE; i++)
		{
			int conn = 0;
			conn = accept(server_sockfd, (struct sockaddr *) &client_addr, &length);

			if (conn > 0)
			{
				if (fork() == 0)
				{
					fd_set fdR;
					FD_ZERO(&fdR);
					FD_SET(conn, &fdR);
					send(conn, buf_send[0], 5, 0);
					client_status[i]++;
					while (1)
					{
						memset(buf_recv, 0, sizeof(buf_recv));
						if (client_status[i] == 1)
						{
							recv(conn, &stuno[i], 4, 0);
							stuno[i] = ntohl(stuno[i]);
							send(conn, buf_send[client_status[i]], 3, 0);
							client_status[i]++;
						}
						else if (client_status[i] == 2)
						{
							recv(conn, &pid[i], 4, 0);
							pid[i] = ntohl(pid[i]);
							send(conn, buf_send[client_status[i]], 5, 0);
							client_status[i]++;
						}
						else if (client_status[i] == 3)
						{
							int len = recv(conn, buf_recv, sizeof(buf_recv), 0);
							if (len != 19)
								close(conn);
							else
							{
								for (j = 0; j < 19; j++)
									TIME[i][j] = buf_recv[j];
								send(conn, buf_send[client_status[i]], 9, 0);
								client_status[i]++;
							}
						}
						else if (client_status[i] == 4)
						{
							recv(conn, buf_recv, sizeof(buf_recv), 0);
							for (j = 0; j < randint; j++)
								random[i][j] = buf_recv[j];
							send(conn, buf_send[client_status[i]], 3, 0);
							client_status[i]++;
						}

						else if (client_status[i] == 5)
						{
							while (1)
							{
								memset(buf_recv, 0, sizeof(buf_recv));
								if (!recv(conn, buf_recv, sizeof(buf_recv), 0))
								{
									char filename[100];
									sprintf(filename, "1551265.%d.pid.txt", pid[i]);
									ofstream fout(filename);
									if (fout.is_open())
									{
										fout << stuno[i] << endl << pid[i] << endl;
										for (j = 0; j < 19; j++)
											fout << TIME[i][j];
										fout << endl;
										for (j = 0; j < randint; j++)
											fout << random[i][j];
									}
									fout.close();
									return 0;
								}
							}
						}
					}
				}
			}

		}
	}


	else if (if_fork && !if_block)
	{
		int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		int client_status = 0;
		int enable = 1;
		if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
			perror("setsockopt(SO_REUSEADDR) failed");


		int flags1 = fcntl(server_sockfd, F_GETFL, 0);
		fcntl(server_sockfd, F_SETFL, flags1 | O_NONBLOCK);

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

		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		fd_set fdR;

		for (i = 0; i < QUEUE; i++)
		{
			struct sockaddr_in client_address;
			socklen_t address_len;
			FD_ZERO(&fdR);
			FD_SET(server_sockfd, &fdR);
			int conn = 0;
			if (select(server_sockfd + 1, &fdR, NULL, NULL, 0) > 0)
				conn = accept(server_sockfd, (struct sockaddr *) &client_addr, &length);
			if (conn > 0)
			{
				if (fork() == 0)
				{
					int flags = fcntl(conn, F_GETFL, 0);
					fcntl(conn, F_SETFL, flags | O_NONBLOCK);
					// close(server_sockfd);
					send(conn, buf_send[0], 5, 0);
					client_status++;

					memset(buf_recv, 0, sizeof(buf_recv));
					FD_ZERO(&fdR);
					FD_SET(conn, &fdR);
					while (1)
					{
						if (select(conn + 1, &fdR, NULL, NULL, 0) > 0)
						{
							memset(buf_recv, 0, sizeof(buf_recv));
							if (client_status == 1)
							{
								recv(conn, &stuno[i], 4, 0);
								stuno[i] = ntohl(stuno[i]);
								send(conn, buf_send[client_status], 3, 0);
								client_status++;
							}
							else if (client_status == 2)
							{
								recv(conn, &pid[i], 4, 0);
								pid[i] = ntohl(pid[i]);
								send(conn, buf_send[client_status], 5, 0);
								client_status++;
							}
							else if (client_status == 3)
							{
								int len = recv(conn, buf_recv, sizeof(buf_recv), 0);
								if (len != 19)
									close(conn);
								else
								{
									for (j = 0; j < 19; j++)
										TIME[i][j] = buf_recv[j];
									send(conn, buf_send[client_status], 9, 0);
									client_status++;
								}
							}
							else if (client_status == 4)
							{
								recv(conn, buf_recv, sizeof(buf_recv), 0);
								for (j = 0; j < randint; j++)
									random[i][j] = buf_recv[j];
								send(conn, buf_send[client_status], 3, 0);
								client_status++;
							}
							else if (client_status == 5)
							{
								if (recv(conn, buf_recv, sizeof(buf_recv), 0) <= 0)
								{
									char filename[100];
									sprintf(filename, "1551265.%d.pid.txt", pid[i]);
									ofstream fout(filename);
									if (fout.is_open())
									{
										fout << stuno[i] << endl << pid[i] << endl;
										for (j = 0; j < 19; j++)
											fout << TIME[i][j];
										fout << endl;
										for (j = 0; j < randint; j++)
											fout << random[i][j];
									}
									fout.close();
									return 0;
								}

							}
						}
					}

				}
			}

		}
	}
}
