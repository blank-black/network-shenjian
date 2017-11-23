#include <signal.h>  
#include <stdio.h> 
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>

int pid=0;
char buffer[20];
void ouch(int sig)  
{  
    FILE *data;
    data=fopen("data","rb+");
    memset(buffer,0,sizeof(buffer));
    fgets(buffer,sizeof(buffer),data);
    printf("read:%s\n",buffer);
    fclose(data);

    data=fopen("data","w+");
    fputs("hello world!",data);
    printf("write:hello world!\n");
    fclose(data);
    sleep(1);
    kill(pid,SIGUSR1);
}  

int main()  
{  
    pid_t pid1;
    pid1=fork();
    if(pid1==-1||pid1>0)
        return 0;   
    //改变终端中断信号SIGINT的默认行为，使之执行ouch函数  
    //而不是终止程序的执行  
    while(pid<=0)
    {
        FILE *fp = popen("ps -e | grep \'test3-2-1\' | awk \'{print $1}\'","r");
        char buf[10] = {0};
        fgets(buf, 10, fp);
        pid=atoi(buf);
    }
    sleep(1);
    (void) signal(SIGUSR1, ouch); 
    kill(pid,SIGUSR1);
    while(1)  
        sleep(1);  
}  