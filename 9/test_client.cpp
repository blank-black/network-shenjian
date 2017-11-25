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
#define BUFSIZE 100000
#define QUEUE 1000
#define PERTIME 1

void func_waitpid(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0);
}
struct sockaddr_in servaddr[QUEUE];
struct sockaddr_in server_sockaddr;

int init_socket(int j,int sock_cli[],int pid[],fd_set fdR[],int port,int if_block)
{
	sock_cli[j]=socket(AF_INET,SOCK_STREAM, 0);
	pid[j]=htonl(getpid()+j);
	memset(&servaddr[j], 0, sizeof(servaddr[j]));
	servaddr[j].sin_family = AF_INET;
	servaddr[j].sin_port = htons(port);
	servaddr[j].sin_addr.s_addr = server_sockaddr.sin_addr.s_addr;
	if(!if_block)
	{
		int flags = fcntl(sock_cli[j], F_GETFL, 0);
		fcntl(sock_cli[j], F_SETFL, flags | O_NONBLOCK);
		FD_ZERO(&fdR[j]);
		FD_SET(sock_cli[j], &fdR[j]);
	}
}

int main(int argc ,char* argv[])
{
	int pidd = fork();
	if (pidd > 0)
		return 0;
	signal(SIGCHLD,&func_waitpid);
	int num = argc - 1, port = -1, if_tcp = 1, if_block = 0, if_fork = 0, num_conn = 500, if_ip = 0;
	server_sockaddr.sin_family = AF_INET;
	int i, j, k, n;
	for (n = 1; n <= num; n++)
	{
		if (!strcmp(argv[n], "-ip"))
		{
			n++;
			if_ip = 1;
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
		else if (!strcmp(argv[n], "-num"))
		{
			n++;
			num_conn = atoi(argv[n]);
		}
		else
			return -1;
	}
	if (!if_fork && if_block)
		if_block = 0;
	if (port == -1 || !if_ip)
		return -1;

	int randint;
	char buf_send[10][BUFSIZE], buf_recv[BUFSIZE];
	int stuno[QUEUE] = {0};
	int pid[QUEUE] = {0};
	char TIME[QUEUE][19];
	char **random = new char *[QUEUE];
	for (int i = 0; i < QUEUE; i++)
		random[i] = new char[99999];
	for (j = 0; j < QUEUE; j++)
	{
		stuno[j] = htonl(1551265);
		time_t timep;
		struct tm *p;
		time(&timep);
		p = localtime(&timep);
		sprintf(TIME[j], "%d-%d-%d %d:%d:%d", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min,
		        p->tm_sec);
		TIME[j][0] = (1900 + p->tm_year) / 1000 + '0';
		TIME[j][1] = (1900 + p->tm_year) / 100 % 10 + '0';
		TIME[j][2] = (1900 + p->tm_year) / 10 % 10 + '0';
		TIME[j][3] = (1900 + p->tm_year) % 10 + '0';
		TIME[j][4] = '-';
		TIME[j][5] = (1 + p->tm_mon) / 10 + '0';
		TIME[j][6] = (1 + p->tm_mon) % 10 + '0';
		TIME[j][7] = '-';
		TIME[j][8] = p->tm_mday / 10 + '0';
		TIME[j][9] = p->tm_mday % 10 + '0';
		TIME[j][10] = ' ';
		TIME[j][11] = p->tm_hour / 10 + '0';
		TIME[j][12] = p->tm_hour % 10 + '0';
		TIME[j][13] = ':';
		TIME[j][14] = p->tm_min / 10 + '0';
		TIME[j][15] = p->tm_min % 10 + '0';
		TIME[j][16] = ':';
		TIME[j][17] = p->tm_sec / 10 + '0';
		TIME[j][18] = p->tm_sec % 10 + '0';
	}


	if (!if_fork && !if_block)
	{
		int sock_cli[QUEUE], client_status[QUEUE] = {0};
		fd_set fdR[QUEUE];
		for (j = 0; j < num_conn; j++)
			init_socket(j, sock_cli, pid, fdR, port, 0);
		for (j = 0; j < num_conn; j++)
			if (select(sock_cli[j] + 1, &fdR[j], NULL, NULL, 0) > 0)
				connect(sock_cli[j], (struct sockaddr *) &servaddr[j], sizeof(servaddr[j]));

		while (1)
		{
			for (i = 0; i < num_conn; i++)
			{
				FD_ZERO(&fdR[i]);
				FD_SET(sock_cli[i], &fdR[i]);
				if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
				{
					memset(buf_recv, 0, sizeof(buf_recv));
					int len = recv(sock_cli[i], buf_recv, sizeof(buf_recv), 0);
					if (len > 0)
					{
						if (client_status[i] == 0)
						{
							if (buf_recv[0] == 'S' && buf_recv[1] == 't' && buf_recv[2] == 'u' && buf_recv[3] == 'N' &&
							    buf_recv[4] == 'o')
							{
								send(sock_cli[i], &stuno[i], 4, 0);
								client_status[i]++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
							}

						}
						else if (client_status[i] == 1)
						{
							if (buf_recv[0] == 'p' && buf_recv[1] == 'i' && buf_recv[2] == 'd')
							{
								pid[i] = htonl(getpid() + i);
								send(sock_cli[i], &pid[i], 4, 0);
								client_status[i]++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status[i] = 0;
							}
						}
						else if (client_status[i] == 2)
						{
							if (!strcmp(buf_recv, "TIME"))
							{
								send(sock_cli[i], TIME[i], sizeof(TIME[i]), 0);
								client_status[i]++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status[i] = 0;
							}
						}
						else if (client_status[i] == 3)
						{
							if (buf_recv[0] == 's' && buf_recv[1] == 't' && buf_recv[2] == 'r'
							    && strlen(buf_recv) == 8)
							{
								randint = buf_recv[3] * 10000 + buf_recv[4] * 1000 + buf_recv[5] * 100 +
								          buf_recv[6] * 10 + buf_recv[7] - 11111 * '0';
								for (k = 0; k < randint; k++)
									random[i][k] = rand() % 256;
								send(sock_cli[i], random[i], randint, 0);
								client_status[i]++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status[i] = 0;
							}
						}
						else if (client_status[i] == 4)
						{
							if (buf_recv[0] == 'e' && buf_recv[1] == 'n' && buf_recv[2] == 'd')
							{
								char filename[100];
								sprintf(filename, "1551265.%d.pid.txt", ntohl(pid[i]));
								ofstream fout(filename);
								if (fout.is_open())
								{
									fout << ntohl(stuno[i]) << endl << ntohl(pid[i]) << endl;
									for (j = 0; j < 19; j++)
										fout << TIME[i][j];
									fout << endl;
									for (j = 0; j < randint; j++)
										fout << random[i][j];
								}
								fout.close();
								close(sock_cli[i]);
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status[i] = 0;
							}
						}
						else if (len < 0)
						{
							close(sock_cli[i]);
							init_socket(i, sock_cli, pid, fdR, port, 0);
							if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
							client_status[i] = 0;
						}

					}
				}

			}
		}
	}

	if (if_fork && !if_block)
	{
		int sock_cli[QUEUE], client_status = 0;
		fd_set fdR[QUEUE];
		for (j = 0; j < num_conn; j++)
			init_socket(j, sock_cli, pid, fdR, port, 0);
		for (i = 0; i < num_conn; i++)
		{
			if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
			{
				connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
				if (fork() == 0)
				{
					FD_ZERO(&fdR[i]);
					FD_SET(sock_cli[i], &fdR[i]);
					while (1)
					{
						if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
						{
							memset(buf_recv, 0, sizeof(buf_recv));
							int len = recv(sock_cli[i], buf_recv, sizeof(buf_recv), 0);
							if (len > 0)
							{
								if (client_status == 0)
								{
									if (buf_recv[0] == 'S' && buf_recv[1] == 't' && buf_recv[2] == 'u'
									    && buf_recv[3] == 'N' && buf_recv[4] == 'o')
									{
										send(sock_cli[i], &stuno[i], 4, 0);
										client_status++;
									}
									else
									{
										close(sock_cli[i]);
										init_socket(i, sock_cli, pid, fdR, port, 0);
										if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
											connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
									}

								}
								else if (client_status == 1)
								{
									if (buf_recv[0] == 'p' && buf_recv[1] == 'i' && buf_recv[2] == 'd')
									{
										pid[i] = htonl(getpid());
										send(sock_cli[i], &pid[i], 4, 0);
										client_status++;
									}
									else
									{
										close(sock_cli[i]);
										init_socket(i, sock_cli, pid, fdR, port, 0);
										if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
											connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
										client_status = 0;
									}
								}
								else if (client_status == 2)
								{
									if (!strcmp(buf_recv, "TIME"))
									{
										send(sock_cli[i], TIME[i], sizeof(TIME[i]), 0);
										client_status++;
									}
									else
									{
										close(sock_cli[i]);
										init_socket(i, sock_cli, pid, fdR, port, 0);
										if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
											connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
										client_status = 0;
									}
								}
								else if (client_status == 3)
								{
									if (buf_recv[0] == 's' && buf_recv[1] == 't' && buf_recv[2] == 'r' &&
									    strlen(buf_recv) == 8)
									{
										randint = buf_recv[3] * 10000 + buf_recv[4] * 1000 + buf_recv[5] * 100 +
										          buf_recv[6] * 10 + buf_recv[7] - 11111 * '0';
										for (k = 0; k < randint; k++)
											random[i][k] = rand() % 256;
										send(sock_cli[i], random[i], randint, 0);
										client_status++;
									}
									else
									{
										close(sock_cli[i]);
										init_socket(i, sock_cli, pid, fdR, port, 0);
										if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
											connect(sock_cli[i], (struct sockaddr *) &servaddr[i],
											        sizeof(servaddr[i]));
										client_status = 0;
									}
								}
								else if (client_status == 4)
								{
									if (buf_recv[0] == 'e' && buf_recv[1] == 'n' && buf_recv[2] == 'd')
									{
										char filename[100];
										sprintf(filename, "1551265.%d.pid.txt", ntohl(pid[i]));
										ofstream fout(filename);
										if (fout.is_open())
										{
											fout << ntohl(stuno[i]) << endl << ntohl(pid[i]) << endl;
											for (j = 0; j < 19; j++)
												fout << TIME[i][j];
											fout << endl;
											for (j = 0; j < randint; j++)
												fout << random[i][j];
										}
										fout.close();
										close(sock_cli[i]);
										return 0;
									}
									else
									{
										close(sock_cli[i]);
										init_socket(i, sock_cli, pid, fdR, port, 0);
										if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
											connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
										client_status = 0;
									}
								}
							}
							else if (len < 0)
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 0);
								if (select(sock_cli[i] + 1, &fdR[i], NULL, NULL, 0) > 0)
									connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status = 0;
							}
						}
					}
				}
			}
		}
	}
	if (if_fork && if_block)
	{
		int sock_cli[QUEUE], client_status = 0;
		fd_set fdR[QUEUE];
		for (j = 0; j < num_conn; j++)
			init_socket(j, sock_cli, pid, fdR, port, 1);
		for (i = 0; i < num_conn; i++)
		{
			connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
			if (fork() == 0)
			{
				while (1)
				{
					memset(buf_recv, 0, sizeof(buf_recv));
					int len = recv(sock_cli[i], buf_recv, sizeof(buf_recv), 0);
					if (len > 0)
					{
						if (client_status == 0)
						{
							if (buf_recv[0] == 'S' && buf_recv[1] == 't' && buf_recv[2] == 'u' &&
							    buf_recv[3] == 'N' && buf_recv[4] == 'o')
							{
								send(sock_cli[i], &stuno[i], 4, 0);
								client_status++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 1);
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
							}

						}
						else if (client_status == 1)
						{
							if (buf_recv[0] == 'p' && buf_recv[1] == 'i' && buf_recv[2] == 'd')
							{
								pid[i] = htonl(getpid());
								send(sock_cli[i], &pid[i], 4, 0);
								client_status++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 1);
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status = 0;
							}
						}
						else if (client_status == 2)
						{
							if (!strcmp(buf_recv, "TIME"))
							{
								send(sock_cli[i], TIME[i], sizeof(TIME[i]), 0);
								client_status++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 1);
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status = 0;
							}
						}
						else if (client_status == 3)
						{
							if (buf_recv[0] == 's' && buf_recv[1] == 't' && buf_recv[2] == 'r' && strlen(buf_recv) == 8)
							{
								randint = buf_recv[3] * 10000 + buf_recv[4] * 1000 + buf_recv[5] * 100
								          + buf_recv[6] * 10 + buf_recv[7] - 11111 * '0';
								for (k = 0; k < randint; k++)
									random[i][k] = rand() % 256;
								send(sock_cli[i], random[i], randint, 0);
								client_status++;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 1);
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status = 0;
							}
						}
						else if (client_status == 4)
						{
							if (buf_recv[0] == 'e' && buf_recv[1] == 'n' && buf_recv[2] == 'd')
							{
								char filename[100];
								sprintf(filename, "1551265.%d.pid.txt", ntohl(pid[i]));
								ofstream fout(filename);
								if (fout.is_open())
								{
									fout << ntohl(stuno[i]) << endl << ntohl(pid[i]) << endl;
									for (j = 0; j < 19; j++)
										fout << TIME[i][j];
									fout << endl;
									for (j = 0; j < randint; j++)
										fout << random[i][j];
								}
								fout.close();
								close(sock_cli[i]);
								return 0;
							}
							else
							{
								close(sock_cli[i]);
								init_socket(i, sock_cli, pid, fdR, port, 1);
								connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
								client_status = 0;
							}
						}
						else if (len < 0)
						{
							close(sock_cli[i]);
							init_socket(i, sock_cli, pid, fdR, port, 1);
							connect(sock_cli[i], (struct sockaddr *) &servaddr[i], sizeof(servaddr[i]));
							client_status = 0;
						}
					}
				}
			}
		}
	}
}


