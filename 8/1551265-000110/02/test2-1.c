#include <stdio.h>  
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

#define FIFO_PATH   "./fifo"

void handler(int sig)
{
    printf("sig=%d\n", sig);
}


int main()  
{  
    pid_t pid1;
    pid1=fork();
    if(pid1==-1||pid1>0)
        return 0;   
    signal(SIGPIPE, handler);
    unlink(FIFO_PATH);          // 删除原来的管道
    mkfifo(FIFO_PATH, 0644);    // 创建管道

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork error");
        return -1;
    }

    if (pid) 
    {     // 父进程
        int fd;
        fd = open(FIFO_PATH, O_WRONLY);
        write(fd, "hello world!", 12);
        close(fd);
        while(1)
        {
            sleep(1);
        }

    } 
    else 
    {            // 子进程
        int fd;
        char buf[128] = {0};
        fd = open(FIFO_PATH, O_RDONLY);
        int ret = read(fd, buf, sizeof(buf));

        printf("%s\n", buf);
        close(fd);
        while(1)
        {
            sleep(1);
        }
    }

    return 0;
}