#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
        printf( "child %d exit\n", pid );
    }
    return;
}

int sub() 
{
char str[1024*10]={20};
for(;;)
    sleep(20);
}
int main(int argc,char *argv[])
{
signal(SIGCHLD, &func_waitpid);
    int i,j,num=0;
for(i=0;argv[1][i];i++)
num=num*10+argv[1][i]-'0';
pid_t pid1;
    pid1=fork();
    if(pid1==-1)
    return 0;
    if(pid1==0)
    {

    //fork a child process
    for(i=0;i<num;)
    {
    pid_t pid = fork();
    if (pid == 0)
    {
        sub();        
   	return 0;
    }
    if(pid==-1)
    {
        printf("共分裂%d个子进程\n",i+1 );
        break;
    }
    if(!(i%100))
        printf("已分裂%d个子进程\n",i+1);
        i++;
    }
    while(1)
    {
      sleep(5);
    }
    return 0;
}
if(pid1>0)
return 0;

}
