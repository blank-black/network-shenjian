#include <signal.h> 
#include <stdlib.h> 
#include <stdio.h>  
#include <sys/types.h>
#include <unistd.h>  
#include <string.h>

int pid=0;
char buffer[20];
void ouch1(int sig)  
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

    while(pid<=0)
    {
        FILE *fp = popen("ps -e | grep \'test3-2-2\' | awk \'{print $1}\'","r");
        char buf[10] = {0};
        fgets(buf, 10, fp);
        pid=atoi(buf);
    }
    (void) signal(SIGUSR1, ouch1); 
    while(1)
        sleep(1);
}  

