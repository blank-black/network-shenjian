#include <signal.h> 
#include <stdlib.h> 
#include <stdio.h>  
#include <unistd.h>  
#include <string.h>
    
int main()  
{  
    pid_t pid1;
    pid1=fork();
    if(pid1==-1||pid1>0)
        return 0;   
    //改变终端中断信号SIGINT的默认行为，使之执行ouch函数  
    //而不是终止程序的执行  
    int pid;
    FILE *fp = popen("ps -e | grep \'test3-1-2\' | awk \'{print $1}\'","r");
    char buffer[10] = {0};
    fgets(buffer, 10, fp);
    pid=atoi(buffer);
    kill(pid,SIGUSR1);
    sleep(10);
    kill(pid,SIGUSR2);
    while(1)
        sleep(1);
}  

