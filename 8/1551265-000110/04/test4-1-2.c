#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <signal.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/msg.h>  
int msgid = -1;  

struct msg_st  
{  
    long int msg_type;  
    char text[BUFSIZ];  
};  
  
void ouch(int sig)  
{  
    if(msgctl(msgid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(0);
}  

int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    struct msg_st data;  
    long int msgtype = 0; //注意1  
    (void) signal(SIGINT, ouch);
    (void) signal(SIGQUIT, ouch);
    (void) signal(SIGTERM, ouch);
    //建立消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    while(1)  
    {  
        if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1)  
        {  
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            exit(EXIT_FAILURE);  
        }  
        printf("recv:%s\n",data.text);  
    }  
}  