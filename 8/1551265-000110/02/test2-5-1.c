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
    const char *fifo_name1 = "./my_fifo1";  
    const char *fifo_name2 = "./my_fifo2";  
    int pipe_fd1 = -1;  
    int pipe_fd2 = -1;  
    int res = 0;  
    const int open_mode1 = O_WRONLY;  
    const int open_mode2 = O_RDONLY;
    int bytes_sent = 0;  
    char buffer[BUFSIZE]="Hello world!";  
  
    if(access(fifo_name1, F_OK) == -1)  
    {  
        res = mkfifo(fifo_name1, 0644);  
        if(res != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", fifo_name1);  
            exit(EXIT_FAILURE);  
        }  
    }  
    res=0;
    if(access(fifo_name2, F_OK) == -1)  
    {  
        res = mkfifo(fifo_name2, 0645);  
        if(res != 0)  
        {  
            fprintf(stderr, "Could not create fifo %s\n", fifo_name2);  
            exit(EXIT_FAILURE);  
        }  
    }
  
    // printf("Process %d opening FIFO O_WRONLY\n", getpid());  
    pipe_fd1 = open(fifo_name1, open_mode1);  
    pipe_fd2 = open(fifo_name2, open_mode2);  

    if(pipe_fd1 != -1)  
    {  
        res = write(pipe_fd1, buffer, sizeof(buffer));  
        printf("write:%s\n",buffer);
        close(pipe_fd1);  
    }  
    else  
        exit(EXIT_FAILURE);  
    if(pipe_fd2 != -1)  
    {  
        res = read(pipe_fd2, buffer, sizeof(buffer)); 
        printf("read:%s\n",buffer);
        close(pipe_fd2);  
    }
    else 
        exit(EXIT_FAILURE); 
    while(1)
    {
        sleep(1);
    }
}  