#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h> 
#include <sys/types.h>
#include <signal.h>  
#include <errno.h>  
#include <sys/msg.h>  
#define MAX_TEXT 512  

int msgid1 = -1,msgid2=-1;  

struct msg_st  
{  
    long int msg_type;  
    char text[BUFSIZ];  
};  
void ouch(int sig)  
{  
    if(msgctl(msgid1, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    if(msgctl(msgid2, IPC_RMID, 0) == -1)  
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
    struct msg_st data1,data2;  
    long int msgtype = 0; //注意1  
  
    //建立消息队列  
    msgid1 = msgget((key_t)4321, 0777 | IPC_CREAT);  
    if(msgid1 == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    msgid2 = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid2 == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    (void) signal(SIGINT, ouch);
    (void) signal(SIGQUIT, ouch);
    (void) signal(SIGTERM, ouch);

    while(1)  
    {  
        data1.msg_type = 1;    //注意2  
        strcpy(data1.text, "hello world2!");  
        printf("send:%s\n",data1.text);
        //向队列发送数据  
        if(msgsnd(msgid1, (void*)&data1, MAX_TEXT, 0) == -1)  
        {  
            fprintf(stderr, "msgsnd failed\n");  
            exit(EXIT_FAILURE);  
        }  

        memset(data2.text,0,sizeof(data2.text));
        if(msgrcv(msgid2, (void*)&data2, BUFSIZ, msgtype, 0) == -1)  
        {  
            fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            exit(EXIT_FAILURE);  
        }  
        printf("recv:%s\n",data2.text); 

        sleep(1);  
    }  
}  