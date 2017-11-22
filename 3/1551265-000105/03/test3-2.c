#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

sig_atomic_t child_exit_status;

void clean_up_child_process(int signal_num)
{
    /* clean up child process */
    int status;
    wait (&status);

    /* store its exit status in a global variable */
    child_exit_status = status;
}

int main()
{
    struct sigaction sigchild_action;
    memset(&sigchild_action, 0, sizeof(sigchild_action));
    sigchild_action.sa_handler = &clean_up_child_process;
    sigaction(SIGCHLD, &sigchild_action, NULL);
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
        for(j=0;j<3;j++)
        {
            printf("%d  %d  1551265  sub\n",(int)getppid(),(int)getpid());
            sleep(25);

        }
	return 0;
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
