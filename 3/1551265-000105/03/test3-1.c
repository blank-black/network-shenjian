#include <stdio.h>
#include <sys/types.h>

int main()
{
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
