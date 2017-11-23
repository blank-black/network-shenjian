#include <signal.h>  
#include <stdio.h> 
#include <stdlib.h>  
#include <unistd.h>  
  
void ouch1(int sig)  
{  
    printf("\nOUCH1! - I got signal %d\n", sig);       
}  

void ouch2(int sig)  
{  
    printf("\nOUCH2! - I got signal %d\n", sig);  
    exit(0);
}  

void ouch3(int sig)
{
    printf("\nOUCH3! - I got signal %d\n", sig); 
    (void) signal(SIGINT, SIG_DFL); 
}

int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    //改变终端中断信号SIGINT的默认行为，使之执行ouch函数  
    //而不是终止程序的执行  
    (void) signal(SIGUSR1, ouch1); 
    (void) signal(SIGUSR2, ouch2);
    (void) signal(SIGINT, ouch3);
    while(1)  
        sleep(1);  
}  