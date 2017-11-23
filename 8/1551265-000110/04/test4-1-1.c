#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/msg.h>  
#include <errno.h>  
  
#define MAX_TEXT 512  
struct msg_st  
{  
    long int msg_type;  
    char text[MAX_TEXT];  
};  
  
int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    struct msg_st data;  
    char buffer[BUFSIZ];  
    int msgid = -1;  
  
    //建立消息队列  
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1)  
    {  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
  
    while(1)  
    {  
        data.msg_type = 1;    //注意2  
        printf("send:hello world!\n");
        strcpy(data.text, "hello world!");  
        //向队列发送数据  
        if(msgsnd(msgid, (void*)&data, MAX_TEXT, 0) == -1)  
        {  
            fprintf(stderr, "msgsnd failed\n");  
            exit(EXIT_FAILURE);  
        }  
        sleep(1);  
    }  
}  