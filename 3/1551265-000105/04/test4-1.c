#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

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
        //child process exit, and to be a zombie process
        for(;;)
        {
            printf("%d  %d  1551265  sub\n",(int)getppid(),(int)getpid());
            sleep(15);

        }
    }
        i++;
        sleep(3);
    }
    while(1)
    {
        printf("%d  %d  1551265  main\n",(int)getppid(),(int)getpid());
        sleep(5);
    }
    return 0;
}
if(pid1>0)
return 0;

}

