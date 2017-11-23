#include <unistd.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <limits.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <string.h>  
#define BUFSIZE 100
int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    const char *fifo_name = "./my_fifo";  
    int pipe_fd = -1;  
    int res = 0;  
    const int open_mode = O_WRONLY;  
    int bytes_sent = 0;  
    char buffer[BUFSIZE]="Hello world!";  
  
    if(access(fifo_name, F_OK) == -1)  
    {  
        res = mkfifo(fifo_name, 0644);  
        if(res != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);  
            exit(EXIT_FAILURE);  
        }  
    }  
  
    printf("Process %d opening FIFO O_WRONLY\n", getpid());  
    pipe_fd = open(fifo_name, open_mode);  
    if(pipe_fd != -1)  
    {  
        res = write(pipe_fd, buffer, sizeof(buffer));  
        printf("write:%s\n",buffer);
        close(pipe_fd);  
    }  
    else  
        exit(EXIT_FAILURE);    
    while(1)
    {
        sleep(1);
    }
  
}  