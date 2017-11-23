#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <signal.h>  
#include <sys/shm.h>  

#define text_SZ 2048  

struct shared_use_st  
{  
    int written1;//作为一个标志，非0：表示可读，0表示可写 
    int written2;
    char text1[text_SZ];//记录写入和读取的文本  
    char text2[text_SZ];
};    

void *shm = NULL;  
  
void ouch(int sig)  
{  
    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(0);
}  

int main()  
{  
    pid_t pid;
    pid=fork();
    if(pid==-1||pid>0)
        return 0;   
    struct shared_use_st *shared = NULL;  
    char buf_send[BUFSIZ + 1];  
    strcpy(buf_send,"hello world2!");
    int shmid;  
    (void) signal(SIGINT, ouch);
    (void) signal(SIGQUIT, ouch);
    (void) signal(SIGTERM, ouch);
    shmid = shmget((key_t)4321, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
      
    shm = shmat(shmid, (void*)0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    // printf("Memory attached at %X\n", (int)shm);  
      
    shared = (struct shared_use_st*)shm;  
    while(1)  
    {  
        while(!shared->written2)
        {
            strncpy(shared->text2, buf_send, text_SZ);  
            printf("send:%s\n",buf_send);      
            shared->written2 = 1;  
        }
        while(shared->written1)
        {
            printf("recv:%s\n", shared->text1);  
            shared->written1 = 0;
        }
        sleep(1);
    }  
}  