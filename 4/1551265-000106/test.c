#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <stdarg.h>
#include "read.c"

# define MAXLINE 2048
# define  NGX_OK          0
# define  NGX_ERROR      -1
# define NGX_SETPROCTITLE_PAD       '\0'

int numin=0,numout=0;
int child[101]={0};
int child_pid[101]={0};


typedef int                 ngx_int_t;
typedef unsigned int        ngx_uint_t;
extern char **environ;
static char *ngx_os_argv_last;
int              ngx_argc;
char           **ngx_os_argv;

void func_waitpid(int signo)
{
    pid_t pid;
    int stat;
    while( (pid = waitpid(-1, &stat, WNOHANG)) > 0 )
    {
        numout++;
        int i;
        for(i=0;i<101;i++)
            if(child_pid[i]==pid)
            {
                child[i]=0;
                break;
            }
    }
    return;
}

u_char * ngx_cpystrn(u_char *dst, u_char *src, size_t n)
{
    if (n == 0)
        return dst;
    while (--n)
    {
        *dst = *src;
        if (*dst == '\0')
            return dst;
        dst++;
        src++;
    }
    *dst = '\0';
    return dst;
}

ngx_int_t ngx_init_setproctitle()
{
    u_char      *p;
    size_t       size;
    ngx_uint_t   i;
    size = 0;
    for (i = 0; environ[i]; i++)
        size += strlen(environ[i]) + 1;
    p = malloc(size);
    if (p == NULL)
        return NGX_ERROR;
    ngx_os_argv_last = ngx_os_argv[0];
    for (i = 0; ngx_os_argv[i]; i++)
        if (ngx_os_argv_last == ngx_os_argv[i])
            ngx_os_argv_last = ngx_os_argv[i] + strlen(ngx_os_argv[i]) + 1;
    for (i = 0; environ[i]; i++)
    {
        if (ngx_os_argv_last == environ[i])
        {
            size = strlen(environ[i]) + 1;
            ngx_os_argv_last = environ[i] + size;
            ngx_cpystrn(p, (u_char *) environ[i], size);
            environ[i] = (char *) p;
            p += size;
        }
    }

    ngx_os_argv_last--;

    return NGX_OK;
}

void ngx_setproctitle(char *title)
{
    u_char     *p;
    ngx_os_argv[1] = NULL;
    p = ngx_cpystrn((u_char *) ngx_os_argv[0],(u_char *) "",ngx_os_argv_last - ngx_os_argv[0]);
    p = ngx_cpystrn(p, (u_char *) title, ngx_os_argv_last - (char *) p);
    if (ngx_os_argv_last - (char *) p)
        memset(p, NGX_SETPROCTITLE_PAD, ngx_os_argv_last - (char *) p);
}

static ngx_int_t ngx_save_argv(int argc, char *const *argv)
{
    ngx_os_argv = (char **) argv;
    ngx_argc = argc;
    return NGX_OK;
}

void creat_daemon(void);
void change_str(int n,char str[])
{
    if(n<100)
        str[1]=n%10+'0',str[0]=n/10%10+'0',str[2]='\0';
    else
        str[0]='1',str[1]='0',str[2]='0',str[3]='\0';
}
long start,start_main, end;

void change_title(int n,int flag_time,int flag_main,char title[])
{
    strcpy(title,"");
    char temp[15]="";
    if(!flag_main)
        change_str(n+1,temp);
    if(flag_time)
    {
        end=time(0);
        int use_time;
        if(flag_main)
            use_time=end-start_main;
        else
            use_time=end-start;
        int temp_h=use_time/3600,temp_m=use_time/60%60,temp_s=use_time%60;
        char str_h[5],str_m[5],str_s[5],link[]=":";
        change_str(temp_h,str_h);
        change_str(temp_m,str_m);
        change_str(temp_s,str_s);
        strcat(strcat(strcat(strcat(strcat(strcat(temp," "),str_h),link),str_m),link),str_s);
    }
    if(!flag_main)
        strcpy(title,"./test [sub-");
    else
        strcpy(title,"./test [main");
    strcat(strcat(title,temp),"]");
}


int main(int argc, char *argv[])
{
    int num=read_file(),i;
    signal(SIGCHLD, &func_waitpid);
    pid_t pid1 =fork();
    ngx_save_argv(argc,argv);
    ngx_init_setproctitle();
    char title[30]="";
    if(pid1 == 0)
    {
        start_main = time(0);
        while(1)
        {
            sleep(1);
            while(numin-numout<num)
            {
                int n;
                for(i=0;i<num;i++)
                    if(child[i]==0)
                    {
                        n=i;
                        break;
                    }
                child[n]=1;
                pid_t pid = fork();
                if (pid == 0)//子进程
                {
                    prctl(PR_SET_PDEATHSIG, SIGHUP);
                    start = time(0);
                    change_title(n,0,0,title);
                    prctl(PR_SET_NAME, title);
                    while(1)
                    {
                        change_title(n,1,0,title);
                        ngx_setproctitle(title);
                        sleep(1);
                    }
                    child[n]=0;
                    return 0;
                }
                else if(pid==-1)//异常退出
                    return -1;
                else//父进程
                {
                    child_pid[n]=pid;
                    numin++;
                    change_title(n,0,1,title);
                    prctl(PR_SET_NAME, title);
                    change_title(n,1,1,title);
                    ngx_setproctitle(title);
                    sleep(1);
                }
            }
            change_title(20,1,1,title);
            ngx_setproctitle(title);
        }
    }
    else
        return 0;
}
