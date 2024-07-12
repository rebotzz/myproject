#pragma once
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 将进程持久化，变成一个守护进程，自成一个会话
// 可以使用系统调用:daemon()
void MyDaemon()
{
    // 忽略信号，防止进程关闭
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    // 重定向文件描述符，后台进程没有标准输入输出,重定向到“文件黑洞” /dec/null 无论读取/写入都不进行
    // xshell同时只能让一个进程输入
    int fd = open("/dec/null", O_RDONLY | O_WRONLY);
    dup2(0, fd);
    dup2(1, fd);
    dup2(2, fd);

    // 保证不是进程组的组长，成为子进程
    if(fork() > 0)
    {
        // parent
        exit(0);
    }
    //child

    // 使进程成为一个独立会话，setsid()缺点：进程组长调用会失败
    pid_t id = setsid();
    if(id < 0)
    {
        exit(1);    //fail
    }
}