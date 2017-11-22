#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
int main()
{
while(1)
{
pid_t pid;
pid=fork();
if(pid==-1)
return 0;
if(pid==0)
{
printf("1551265\n");
fflush(stdout);
sleep(5);
}
if(pid>0)
return 0;
}
}
