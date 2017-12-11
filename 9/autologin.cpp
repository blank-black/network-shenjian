#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h> 
#include <time.h>


using namespace std;

#define LEN 30000
#define port 80
#define IP "192.168.192.17"

int retry = 0;

//状态请求
char request_status[]="GET /srun_portal_pc_succeed.php HTTP/1.1\r\n\
Host: 192.168.192.18\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
\r\n";

//退出请求
char request_out[2048];
void set_request_out(const char username[])
{
	char buffer[1024] = { 0 };
	sprintf(buffer, "action=auto_logout&info=&user_ip=10.22.241.253&username=1551265");
	int len = strlen(buffer);
	sprintf(request_out, "POST http://192.168.192.18/srun_portal_pc_succeed.php HTTP/1.1\r\n\
Host: 192.168.192.18\r\n\
Connection: keep-alive\r\n\
Content-Length: %u\r\n\
Cache-Control: max-age=0\r\n\
Origin: http://192.168.192.18\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image,/webp,image/apng,*/*;q=0.8\r\n\
Referer: http://192.168.192.18/srun_portal_pc_succeed.php\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9\r\n\
\r\n\
action=auto_logout&info=&user_ip=10.22.241.253&username=1551265", len);
}

//登录请求
char request_in[2048];
void set_request_in(const char username[], const char password[])
{
	char buffer[1024] = { 0 };
	sprintf(buffer, "action=login&username=151265&password={B}MTU5MzU3&ac_id=7&user_ip=10.22.241.253&nas_ip=&user_mac=&save_me=1&ajax=1");
	int len = strlen(buffer);
	sprintf(request_in, "POST http://192.168.192.18/include/auth_action.php HTTP/1.1\r\n\
Host: 192.168.192.18\r\n\
Connection: keep-alive\r\n\
Content-Length: %u\r\n\
Accept: */*\r\n\
Origin: http://192.168.192.18\r\n\
X-Requested-With: XMLHttpRequest\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Referer: http://192.168.192.18/srun_portal_pc.php?ac_id=7&userip=10.22.241.253&uservlan=600&userurl=http://www.msftconnecttest.com/redirect\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9\r\n\
\r\n\
action=login&username=1551265&password={B}MTU5MzU3&ac_id=7&user_ip=10.22.241.253&nas_ip=&user_mac=&save_me=1&ajax=1", len);
}


//获取当前时间

int connect()
{
	int sockfd;
	struct sockaddr_in serv_addr;
	char time[20];
	//建立socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		cout << "socket error" << endl;
		exit(1);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(port);
	get_time(time);
	cout << time << " connecting..." << endl;
	sleep(1);

	char choice;

	while (1)
	{
		if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
		{
			get_time(time);
			cout << time << " connect error" << endl;
			sleep(retry);
		}
		else
		{
			get_time(time);
			cout << time << " connect success" << endl;
			break;
		}
	}
	return sockfd;
}

int if_login(int sockfd)
{
	fd_set rfds, wfds, master;
	int maxfd = sockfd;
	FD_ZERO(&master);
	FD_SET(sockfd, &master);
	int len, flag = 0;
	char buf[LEN];
	char time[20];
	while (1)
	{
		rfds = master;
		if (flag)
		{
			FD_ZERO(&wfds);
		}
		else
		{
			wfds = master;
			flag = 1;
		}
		if (select(maxfd + 1, &rfds, &wfds, NULL, NULL) == -1)
		{
			cout << "select error" << endl;
			exit(1);
		}
		if (FD_ISSET(sockfd, &rfds))
		{

			len = read(sockfd, buf, LEN);
			if (len < 0)
			{
				cout << "read error" << endl;
				close(sockfd);
				exit(1);
			}
			cout << "len=" <<len<< endl;

			int i;
			for(i=0;i<len;i++)
				if(!buf[i])
					buf[i]=1;
			buf[len] = '\0';
				
			// cout << buf << endl;
			char *logout = strstr(buf, "></span></td>");
			if (logout)
			{
				get_time(time);
				cout << time << " out" << endl;
				return 1;
			}
			else
			{
				get_time(time);
				cout << time << " in" << endl;
				return 0;
			}
			sleep(1);
		}
		if (FD_ISSET(sockfd, &wfds))
		{
			len = write(sockfd, request_status, strlen(request_status));
			if (len <= 0)
			{
				cout << "send judge login request error" << endl;
				exit(1);
			}
			else
			{
				get_time(time);
				cout << time << " Send judge login request" << endl;
			}
		}
	}
}

void login(int sockfd)
{
	char time[20];
	get_time(time);
	int len = write(sockfd, request_in, strlen(request_in));
	cout << time << " Send login request" << endl;
	//cout << buffer << endl;
	char buf[LEN];
	len = read(sockfd, buf, LEN);
	get_time(time);
				int i;
			for(i=0;i<len;i++)
				if(!buf[i])
					buf[i]=1;
			buf[len] = '\0';

	char *p = strstr(buf, "login_ok");
	
	// if (p)
        cout<<time << " login success" << endl;
	// else
		// cout << time << " login error" << endl;
		
}

void logout(int sockfd)
{
	char time[20];
	get_time(time);
	int len = write(sockfd, request_out, strlen(request_out));
	cout << time << " Send logout request" << endl;
	char buf[65536];
	len = read(sockfd, buf, 1024);
	get_time(time);
	char *p = strstr(buf, "logout");
	if (p)
	{
		cout << time << " logout success" << endl;
	}
	else
		cout << time << " logout error" << endl;
}

int main(int argc,char** argv)
{
	//读取配置文件
	int sockfd = connect();

	char username[20];
	char password_temp[20];
	char retrytime[10];
	char checktime[10];
	// read_cfg(username, password_temp, retrytime, checktime);
	retry = 15;
	int check = 900;

	char password[10] = "MTU5MzU3";
	cout << "The password after base64 is:" << password << endl;

	//设置连接请求
	set_request_in(username, password);
	set_request_out(username);

	char time[20];
	if(argc==1)
		while(1)
		{
			if(if_login(sockfd))
				login(sockfd);
			else
				cout<<"have login"<<endl;
			sleep(retry);
		}
	else if(argc==2&&!strcmp(argv[1],"logout"))
		logout(sockfd);
	else
		exit(1);
	return 0;
}