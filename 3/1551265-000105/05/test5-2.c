#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int numin=0,numout=0,max=0;
void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) 
	numout++;
    return;
}
int sub()
{
char str[1024*10]={20};
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
    for(i=0;i<num;)
    {
if(numin-numout<1500)
{
    pid_t pid = fork();
    if (pid == 0)
    {	
if(i==num-1)
printf("子进程最大进程号为%d",(int)getpid());
        sub();
        return 0;
    }
   else if(pid==-1)
    {
        printf("共分裂%d个子进程\n",i+1 );
        break;
    }
else
{
numin++;
    if(!(i%1000))
       {
        printf("已分裂%d个子进程\n",i+1);
        sleep(20);
        }
        i++;
}    
}
else
sleep(20);
}
if(i==num)
printf("共分裂%d个子进程\n",i);
    while(1)
    {
      sleep(5);
    }
return 0;}
if(pid1>0)
return 0;
}

