#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream> 
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <cstdio>


#define SRVIP 	"172.30.0.94"
#define SRVPORT 80
#define BUFSIZE	8049

void 	sig_child(int s);
void 	catch_signal();
int 	make_daemon();
char* server_time(char *t);
long  cmd1_len = 1000;
long  cmd2_len = 1000;
long  cmd3_len = 1000;

char cmd1[] = "POST / HTTP/1.1\r\n\
Host: 172.30.0.94\r\n\
Connection: keep-alive\r\n\
Content-Length: 34\r\n\
Cache-Control: max-age=0\r\n\
Origin: http://172.30.0.94\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Referer: http://172.30.0.94/\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
\r\n";

// char cmd2[] = 	"GET / HTTP/1.1\r\n\
// Host: 192.168.2.231\r\n\
// Connection: keep-alive\r\n\
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
// Accept: image/webp,image/*,*/*;q=0.8\r\n\
// Referer: http://192.168.2.231/\r\n\
// Accept-Encoding: gzip, deflate, sdch\r\n\
// Accept-Language: zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4\r\n\
// Cookie: PHPSESSID=mnruqf0miqepp468c4u08gieo0\r\n\
// \r\n";
char cmd2[]="GET /srun_portal_pc.php?ac_id=1& HTTP/1.1\r\n\
Host: 192.168.192.17\r\n\
Connection: keep-alive\r\n\
Cache-Control: max-age=0\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
\r\n";

char cmd3[] = 	"GET /F.htm HTTP/1.1\r\n\
Host: 172.30.0.94\r\n\
Connection: keep-alive\r\n\
Upgrade-Insecure-Requests: 1\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n\
Referer: http://172.30.0.94/\r\n\
Accept-Encoding: gzip, deflate\r\n\
Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n\
";
using namespace std ;


#ifndef ___BASE64_H___
#define ___BASE64_H___


#include <string>

using namespace std;

class CBase64
{
public:
	CBase64();
	~CBase64();

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn		需要进行编码的数据
				[in]uInLen  输入参数的字节数
				[out]strOut 输出的进行base64编码之后的字符串
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const  char *pIn,  long uInLen, string& strOut);

	/*********************************************************
	* 函数说明：将输入数据进行base64编码
	* 参数说明：[in]pIn			需要进行编码的数据
				[in]uInLen		输入参数的字节数
				[out]pOut		输出的进行base64编码之后的字符串
				[out]uOutLen	输出的进行base64编码之后的字符串长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Encode(const  char *pIn,  long uInLen,  char *pOut,  long *uOutLen);
	
	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Decode(const string& strIn,  char *pOut,  long *uOutLen) ;

	/*********************************************************
	* 函数说明：将输入数据进行base64解码
	* 参数说明：[in]strIn		需要进行解码的数据
				[out]pOut		输出解码之后的节数数据
				[out]uOutLen	输出的解码之后的字节数长度
	* 返回值  ：true处理成功,false失败
	* 作  者  ：ChenLi
	* 编写时间：2009-02-17
	**********************************************************/
	bool static Decode(const  char *pIn,  long uInLen,  char *pOut,  long *uOutLen) ;
};

#endif // ___BASE64_H___


static const char *g_pCodes =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static const  char g_pMap[256] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	 37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	 49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

CBase64::CBase64()
{
}

CBase64::~CBase64()
{
}

bool CBase64::Encode(const  char *pIn,  long uInLen,  char *pOut,  long *uOutLen)
{
	 long i, len2, leven;
	 char *p;

	if(pOut == NULL || *uOutLen == 0)
		return false;

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	len2 = ((uInLen + 2) / 3) << 2;
	if((*uOutLen) < (len2 + 1)) return false;

	p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		*p++ = g_pCodes[pIn[0] >> 2];
		*p++ = g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		*p++ = g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		*p++ = g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		 char a = pIn[0];
		 char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		 char c = 0;

		*p++ = g_pCodes[a >> 2];
		*p++ = g_pCodes[((a & 3) << 4) + (b >> 4)];
		*p++ = ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		*p++ = '=';
	}

	*p = 0; // Append NULL byte
	*uOutLen = p - pOut;
	return true;
}

bool CBase64::Encode(const  char *pIn,  long uInLen, string& strOut) 
{
	 long i, len2, leven;
	strOut = "";

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	len2 = ((uInLen + 2) / 3) << 2;
	//if((*uOutLen) < (len2 + 1)) return false;

	//p = pOut;
	leven = 3 * (uInLen / 3);
	for(i = 0; i < leven; i += 3)
	{
		strOut += g_pCodes[pIn[0] >> 2];
		strOut += g_pCodes[((pIn[0] & 3) << 4) + (pIn[1] >> 4)];
		strOut += g_pCodes[((pIn[1] & 0xf) << 2) + (pIn[2] >> 6)];
		strOut += g_pCodes[pIn[2] & 0x3f];
		pIn += 3;
	}

	if (i < uInLen)
	{
		 char a = pIn[0];
		 char b = ((i + 1) < uInLen) ? pIn[1] : 0;
		 char c = 0;

		strOut += g_pCodes[a >> 2];
		strOut += g_pCodes[((a & 3) << 4) + (b >> 4)];
		strOut += ((i + 1) < uInLen) ? g_pCodes[((b & 0xf) << 2) + (c >> 6)] : '=';
		strOut += '=';
	}

	//*p = 0; // Append NULL byte
	//*uOutLen = p - pOut;
	return true;
}

bool CBase64::Decode(const string& strIn,  char *pOut,  long *uOutLen) 
{
	 long t, x, y, z;
	 char c;
	 long g = 3;

	//ASSERT((pIn != NULL) && (uInLen != 0) && (pOut != NULL) && (uOutLen != NULL));

	for(x = y = z = t = 0; x < strIn.length(); x++)
	{
		c = g_pMap[strIn[x]];
		if(c == 255) continue;
		if(c == 254) { c = 0; g--; }

		t = (t << 6) | c;

		if(++y == 4)
		{
			if((z + g) > *uOutLen) { return false; } // Buffer overflow
			pOut[z++] = ( char)((t>>16)&255);
			if(g > 1) pOut[z++] = ( char)((t>>8)&255);
			if(g > 2) pOut[z++] = ( char)(t&255);
			y = t = 0;
		}
	}

	*uOutLen = z;
	return true;
}













/* set child exit handler */
void sig_child(int s) {
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

void catch_signal() {
	signal(SIGCHLD, sig_child); 
}

int make_daemon() {
	switch(fork()) {
		case -1: return -1; /* fail to fork */
		case 0:	break;		/* Child falls here */
		default: _exit(EXIT_SUCCESS);
	}
	
	//if(setsid() == -1)	/* become leader of new session */
	//	return -1;
	
	switch(fork()) {
		case -1: return -1; /* fail to fork */
		case 0: break;
		default: _exit(EXIT_SUCCESS);
	}
	
	umask(0); 	/* Clear file mask mode */
	
	return 0;
}

char* server_time(char *t)
{
	time_t now;
	struct tm * w;
	time(&now);
	w = localtime(&now);
	sprintf(t, "%04d-%02d-%02d %02d:%02d:%02d", w->tm_year + 1900,
			w->tm_mon + 1, w->tm_mday, w->tm_hour, w->tm_min, w->tm_sec);
    return t;
}
char t[50];
char username[256];
char password[256];
int retrytime;
int checktime;

int check_login(int server_sock)
{
	char buf[BUFSIZE];
	fd_set 	rfd, wfd;
	int 	sel, maxfd, len;
	// char cmd_temp[1000];
	// memset(cmd_temp,0,sizeof(cmd_temp));
	// if(CBase64::Decode(cmd2, strlen(cmd2),cmd_temp,&cmd2_len ))
	// 	cout<<"check_login加密成功"<<endl;
	// else
	// 	cout<<"check_login加密失败"<<endl;

	int 	cmdlen = strlen(cmd2);
	int 	sentlen = 0;

	while(1)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);

		FD_SET(server_sock, &rfd);
		if (cmdlen > sentlen)
		{
			FD_SET(server_sock, &wfd);
		}

		maxfd = server_sock;

		sel = select (maxfd + 1, &rfd, &wfd, NULL, NULL);

		if (sel > 0)
		{
			if (FD_ISSET(server_sock, &rfd)) 
			{
				len = recv(server_sock, buf, BUFSIZE, 0);
				cout<<len<<endl;
				if (len <= 0)
				{
					printf(" checklogin - %s Server close connection\n", server_time(t));
					close(server_sock);
					exit(-1);
				}
				buf[len] = 0;
				fprintf(stderr, " checklogin - Received %d bytes from server\n", len);


				char *token_login = strstr(buf, "Login</font>");
				char *token_logout = strstr(buf, "Logout</font>");
				if( token_login )
				{
					printf(" checklogin - %s Status is Logout\n", server_time(t));
					return 0;
				}
				else
				if( token_logout )
				{
					printf(" checklogin - %s Status is Login\n", server_time(t));
					return 1;
				}
			}

			if (FD_ISSET(server_sock, &wfd)) 
			{
				len = send(server_sock, cmd2 + sentlen, cmdlen - sentlen, 0);
				//fprintf(stderr, " S - sent %d bytes to server\n", len);
				sentlen += len;
			}
		}
	}
	return -1;
}

int login_success(char *buf)
{
	char *p = strstr(buf, "You have successfully logged into our system");
	if ( p )
	{
		printf(" login_success - %s Login Server successed\n", server_time(t));
		return 2;
	}
	p = strstr(buf, "ldap auth error");
	if ( p )
	{
		printf(" login_success - %s Login Server Failed (Username/Passwork Error)\n", server_time(t));
		printf(" login_success - %s Exit\n", server_time(t));
		exit(-1);
	}
}

void login(int server_sock)
{
	char buf[BUFSIZE];
	fd_set 	rfd, wfd;
	int 	sel, maxfd, len;
	int 	cmdlen;
	int 	sentlen = 0;
	char cmd[1000];
	// char cmd_temp[1000];
	// memset(cmd_temp,0,sizeof(cmd_temp));

	sprintf(cmd, cmd1, username, password);

	// if(CBase64::Decode(cmd_temp, strlen(cmd_temp), cmd,&cmd1_len))
	// 	cout<<"login加密成功"<<endl;
	// else
	// 	cout<<"login加密失败"<<endl;

	cmdlen = strlen(cmd);

	while(1)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);

		FD_SET(server_sock, &rfd);
		if (cmdlen > sentlen)
		{
			FD_SET(server_sock, &wfd);
		}

		maxfd = server_sock;

		sel = select (maxfd + 1, &rfd, &wfd, NULL, NULL);///!!!!!!!!!!!

		if (sel > 0)
		{
			if (FD_ISSET(server_sock, &rfd)) 
			{
				len = recv(server_sock, buf, BUFSIZE, 0);
				if (len <= 0)
				{
					printf(" login - %s Server close connection\n", server_time(t));
					close(server_sock);
					exit(-1);
				}

				buf[len] = 0;
				fprintf(stderr, " login - Received %d bytes from server\n", len);

				if (login_success(buf))
					return ;

			}

			if (FD_ISSET(server_sock, &wfd)) 
			{
				len = send(server_sock, cmd + sentlen, cmdlen - sentlen, 0);
				printf(" login - %s Send Login Requset String\n", server_time(t));
				fprintf(stderr, " login - sent %d bytes to server\n", len);
				sentlen += len;
			}
		}
	}
}

void logout(int server_sock)
{
	char buf[BUFSIZE];
	fd_set 	rfd, wfd;
	int 	sel, maxfd, len;
	char cmd_temp[1000];
	if(CBase64::Encode(cmd3, strlen(cmd3), cmd_temp,&cmd3_len))
		cout<<"logout加密成功"<<endl;
	else
		cout<<"logout加密失败"<<endl;

	int 	cmdlen = strlen(cmd_temp);
	int 	sentlen = 0;

	while(1)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);

		FD_SET(server_sock, &rfd);
		if (cmdlen > sentlen)
		{
			FD_SET(server_sock, &wfd);
		}

		maxfd = server_sock;

		sel = select (maxfd + 1, &rfd, &wfd, NULL, NULL);

		if (sel > 0)
		{
			if (FD_ISSET(server_sock, &rfd)) 
			{
				len = recv(server_sock, buf, BUFSIZE, 0);
				if (len <= 0)
				{
					printf(" logout - %s Server close connection\n", server_time(t));
					close(server_sock);
					exit(-1);
				}

				buf[len] = 0;
				fprintf(stderr, " logout - Received %d bytes from server\n", len);

				char *p = strstr(buf, "Msg=14;");

				if ( p )
				{
					printf(" logout - %s Logout Server Successfully\n", server_time(t));
					return ;
				}
			}

			if (FD_ISSET(server_sock, &wfd)) 
			{
				len = send(server_sock, cmd_temp + sentlen, cmdlen - sentlen, 0);
				fprintf(stderr, " logout - sent %d bytes to server\n", len);
				sentlen += len;
			}
		}
	}

}

int connect_server()
{
	struct sockaddr_in srvaddr;
	int server_sock;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	inet_aton(SRVIP, &srvaddr.sin_addr);
	srvaddr.sin_port = htons(SRVPORT);


	printf("\n connect_server - %s Connect to Server...\n", server_time(t));

	while (1)
	{
		if(connect(server_sock, (const struct sockaddr*)&srvaddr, sizeof(srvaddr)) < 0)
		{
			printf(" connect_server - %s Cannot connect to Server, wait %d to try\n", server_time(t), retrytime);
			sleep(retrytime);
		}
		else
		{
			printf(" connect_server - %s Connect to Server Successfully\n", server_time(t));
			break;
		}
	}
	return server_sock;
}

void autolog(int argc, char **argv)
{
	/* Logout */
	if (argc > 1 && strcmp(argv[1], "-logout") == 0)	
	{
		int server_sock = connect_server();
		if (check_login(server_sock))
			logout(server_sock);
		return;
	}

	/* Login */
	while(1)
	{	
		int server_sock = connect_server();
		switch (check_login(server_sock))
		{
			case false:
				login(server_sock);
				break;
			case true:
				break;
			default:
				break;
		}
		close(server_sock);
		printf(" autolog - %s Break Connect with Server\n", server_time(t));
		printf(" autolog - %s Sleep %ds\n", server_time(t), checktime);
		sleep(checktime);
	} // end of while
}

void readfile()
{
	char buffer[256];

	ifstream in;
    in.open("autologin.cfg");

    if (!in.is_open())
    {
    	printf("open autologin.cfg failed\n");
    	exit(-1);
    }
    char password_temp[256];
    in.getline(buffer,100);
    sscanf(buffer,"username = %s", &username);  
    in.getline(buffer,100);
    sscanf(buffer,"password = %s", &password_temp);
    long password_len=100;  
    if(CBase64::Encode(password_temp, strlen(password_temp), password,&password_len))
		cout<<"login加密成功"<<endl;
	else
		cout<<"login加密失败"<<endl;

    in.getline(buffer,100);
    sscanf(buffer,"retrytime = %d", &retrytime);  
    in.getline(buffer,100);
    sscanf(buffer,"checktime = %d", &checktime);  
    
    in.close();
}

int main( int argc, char **argv)
{
	make_daemon();
	catch_signal();

	readfile();
	autolog(argc, argv);
}