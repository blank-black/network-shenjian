#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    int pfd1[2],pfd2[2];	//保存打开管道后的两个文件描述符
    pid_t cpid;//保存进程标识符
    char buf;
    if (pipe(pfd1) == -1)//建立管道
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if (pipe(pfd2) == -1)//建立管道
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)//子进程
    {
        sleep(1);
        close(pfd1[0]);          //关闭管道读，引用计数-1 
        write(pfd1[1], "hello world1!", strlen("hello world1!"));
        close(pfd1[1]);//关闭管道写，引用计数-1
        close(pfd2[1]); 
        // write(STDOUT_FILENO, "child:", 7);
        while (read(pfd2[0], &buf, 1) > 0)//从管道循环读取数据
            write(STDOUT_FILENO, &buf, 1);//输出读到的数据
        write(STDOUT_FILENO, "\n", 1);//输出从管道读取的数据
        close(pfd2[0]);
        while(1)
        {
            sleep(1);
        }
    }
    else
    {//父进程
        close(pfd1[1]);         
        // write(STDOUT_FILENO, "parent:", 8);        
        while (read(pfd1[0], &buf, 1) > 0)//从管道循环读取数据
            write(STDOUT_FILENO, &buf, 1);//输出读到的数据
        write(STDOUT_FILENO, "\n", 1);//输出从管道读取的数据
        close(pfd1[0]); 
        // sleep(1);
        close(pfd2[0]);
        sleep(1);
        write(pfd2[1], "hello world2!", strlen("hello world2!"));
        close(pfd2[1]);//关闭管道写，引用计数-1                 
        while(1)
        {
            sleep(1);
        }
    }
}