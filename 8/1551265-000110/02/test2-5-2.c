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
    const char *fifo_name1 = "./my_fifo1";  
    const char *fifo_name2 = "./my_fifo2";  

    int pipe_fd1 = -1;
    int pipe_fd2 = -1;  
    int res = 0;  
    int open_mode1 = O_RDONLY;  
    int open_mode2 = O_WRONLY;  
    char buffer[BUFSIZE];  
    //清空缓冲数组  
    memset(buffer, 0, sizeof(buffer));  
  
    // printf("Process %d opening FIFO O_RDONLY\n", getpid());  
    //以只读阻塞方式打开管道文件，注意与fifowrite.c文件中的FIFO同名  
    pipe_fd1 = open(fifo_name1, open_mode1);
    pipe_fd2 = open(fifo_name2, open_mode2);  
    if(pipe_fd1 != -1)  
    {  
        res = read(pipe_fd1, buffer, sizeof(buffer)); 
        printf("read:%s\n",buffer);
        close(pipe_fd1);  
    }  
    else  
        exit(EXIT_FAILURE);  
    if(pipe_fd2 != -1)  
    {  
        res = write(pipe_fd2, buffer, sizeof(buffer));  
        printf("write:%s\n",buffer);
        close(pipe_fd2);  
    }  
    else  
        exit(EXIT_FAILURE);  
    while(1)
    {
        sleep(1);
    }
}  