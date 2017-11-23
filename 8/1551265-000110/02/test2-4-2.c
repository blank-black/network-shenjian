#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <limits.h>  
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
    int open_mode = O_RDONLY;  
    char buffer[BUFSIZE];  
    //清空缓冲数组  
    memset(buffer, 0, sizeof(buffer));  
  
    printf("Process %d opening FIFO O_RDONLY\n", getpid());  
    //以只读阻塞方式打开管道文件，注意与fifowrite.c文件中的FIFO同名  
    pipe_fd = open(fifo_name, open_mode);  
    if(pipe_fd != -1)  
    {  
        res = read(pipe_fd, buffer, sizeof(buffer)); 
        printf("read:%s\n",buffer);
        close(pipe_fd);  
    }  
    else  
        exit(EXIT_FAILURE);  
    while(1)
    {
        sleep(1);
    }
}  