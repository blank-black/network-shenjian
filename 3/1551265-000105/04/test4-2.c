#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/prctl.h>

void func_waitpid(int signo) {
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) ;
    return;
}

int main()
{
signal(SIGCHLD, &func_waitpid);
    int i,j;
    pid_t pid1;
    pid1=fork();
    if(pid1==-1)
    return 0;
    if(pid1==0)
    {

    //fork a child process
    for(i=0;i<10;)
    {
    pid_t pid = fork();
    if (pid == 0)
    {
        while(1)
        {
	    prctl(PR_SET_PDEATHSIG, SIGHUP);
            printf("%d  %d  1551265  sub\n",(int)getppid(),(int)getpid());
            sleep(15);
        }
        exit(0);
    }
        i++;
        sleep(3);
    }
    while(1)
    {
        printf("%d  %d  1551265  main\n",(int)getppid(),(int)getpid());
        sleep(5);
    }
exit(0);
    system("killall test4-2");
}
if(pid1>0)
return 0;

}
