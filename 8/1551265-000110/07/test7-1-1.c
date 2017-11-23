#include <unistd.h>  
#include <sys/file.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <stdlib.h> 
#include <string.h>
  

int lock_set(int fd, int type)  
{  
    struct flock old_lock, lock;  
    lock.l_whence = SEEK_SET;  
    lock.l_start = 0;  
    lock.l_len = 0;  
    lock.l_type = type;  
    lock.l_pid = -1;  
      
    // 判断文件是否可以上锁   
    fcntl(fd, F_GETLK, &lock);  
      
    if (lock.l_type != F_UNLCK)  
    {  
        //判断文件不能上锁的原因 
        if (lock.l_type == F_RDLCK) 
            printf("Read lock already set by %d\n", lock.l_pid);  
        else if (lock.l_type == F_WRLCK)   
            printf("Write lock already set by %d\n", lock.l_pid);  
    }  
      
    lock.l_type = type;  
    //根据不同的type值进行阻塞式上锁或解锁 
    if ((fcntl(fd, F_SETLKW, &lock)) < 0)  
    {  
        printf("Lock failed:type = %d\n", lock.l_type);  
        return 1;  
    }  
          
    switch(lock.l_type)  
    {  
        case F_RDLCK:  
            printf("Read lock set by %d\n", getpid());  
            break;  
  
        case F_WRLCK:  
            printf("Write lock set by %d\n", getpid());  
            break;  
  
        case F_UNLCK:  
            printf("Release lock by %d\n", getpid());  
            return 1;  
        default:  
            break;  
    }
    return 0;  
}  




int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;  

	int fd;  
	
	fd = open("test7",O_RDWR | O_CREAT, 0666);  
	if(fd < 0)  
	{  
		printf("Open file error\n");  
		exit(1);  
	}  
	  
	/* 给文件上写入锁 */  
	lock_set(fd, F_WRLCK);  
	
	sleep(10);
	int ret;
	char writebuf[20] = "process 1st";
	ret = write(fd, writebuf, strlen(writebuf));
    if (ret < 0)
        printf("write失败.\n");
    else
        printf("write:%d\n", ret);
	/* 给文件解锁 */  
	lock_set(fd, F_UNLCK);  
	close(fd);  
	while(1)  
		sleep(1);
} 